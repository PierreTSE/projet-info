#include "Image.hpp"
#include <iostream>

namespace fs = std::experimental::filesystem;

void Image::loadImage()
{
	if (fs::exists(path_) && fs::is_regular_file(path_))
	{
		cimg_.reset(new cimg_library::CImg<unsigned char>(path_.u8string().c_str()));
	}
	else
		throw std::runtime_error("Invalid path attribute when loading an image.");
}

/* Nomenclature de l'automate pour parser une image :
/  Les mots à parser sont entre guillemets (").
/  Le premier mot correspond au path de l'Image.
/  Tous les autres mots existants correspondent aux tags de l'images, et ce jusqu'à un point virgule (;).
/  Les commentaires sont les lignes commençant par un croisillon (#).
*/

enum State { START, END, QUOTE, PATH_NAME, PATH_BACKSLASH, TAG_NAME, TAG_BACKSLASH, SEMICOLON, OCTOTHORPE, INVALID };

std::istream& operator>>(std::istream& inStream, Image& image)
{
	char c;

	std::string path;
	TagList tagList;
	Tag tag;

	try {
		State state = START;
		while (state != END)
		{
			c = inStream.get();

			switch (state)
			{
			case START:
				if (c == '"')
				{
					state = PATH_NAME;
				}
				else if (c == '#')
				{
					state = OCTOTHORPE;
				}
				else if (c == '\r' || c == '\n' || c == '\v' || c == '\f' || c == '\t' || c == ' ')
				{
					//state = START;
				}
				else if (inStream.eof())
				{
					state = END;
				}
				else if (c == 'ï' || c == '»' || c == '¿') // HACK pour ignorer le byte order mark UTF-8
				{
					//state = START;
				}
				else if (c == 'þ' || c == 'ÿ') // HACK pour ignorer le byte order mark UTF-16
				{
					//state = START;
				}
				else if (c == 0) // HACK pour ignorer le byte order mark UTF-32
				{
					//state = START;
				}
				else
					throw std::runtime_error("Invalid character when looking for block beginning.");
				break;
			case QUOTE:
				if (c == '"')
				{
					state = TAG_NAME;
				}
				else if (c == ';')
				{
					state = SEMICOLON;
				}
				else if (inStream.eof())
				{
					if (state == PATH_NAME)
						throw std::runtime_error("Unexpected end of file in the path name.");
					if (state == TAG_NAME)
						throw std::runtime_error("Unexpected end of file in a tag name at rank " + std::to_string(tagList.size()) + ".");
				}
				else
					state = INVALID;
				break;
			case PATH_NAME:
				if (c == '"')
				{
					state = QUOTE;
				}
				else if (inStream.eof())
				{
					throw std::runtime_error("Unexpected end of file : missing quote after name of path.");
				}
				else if (c == '\\')
				{
					state = PATH_BACKSLASH;
				}
				else
				{
					path += c;
				}
				break;
			case PATH_BACKSLASH:
				if (inStream.eof())
				{
					throw std::runtime_error("Unexpected end of file after escape character in name of path.");
				}
				else
				{
					path += c;
				}
				state = PATH_NAME;
				break;
			case TAG_NAME:
				if (c == '"')
				{
					tagList.emplace(tag);
					tag.erase();
					state = QUOTE;
				}
				else if (inStream.eof())
				{
					throw std::runtime_error("Unexpected end of file : missing quote for name of tag.");
				}
				else if (c == '\\')
				{
					state = TAG_BACKSLASH;
				}
				else
				{
					tag += c;
				}
				break;
			case TAG_BACKSLASH:
				if (inStream.eof())
				{
					throw std::runtime_error("Unexpected end of file after escape character in name of tag.");
				}
				else
				{
					tag += c;
				}
				state = TAG_NAME;
				break;
			case SEMICOLON:
				if (c == '\r' || c == '\n')
				{
					state = END;
				}
				else if (c == '\v' || c == '\f' || c == '\t' || c == ' ')
				{
					//state = SEMICOLON;
				}
				else if (inStream.eof())
				{
					state = END;
				}
				else
					throw std::runtime_error("Invalid character found after end of block.");
				break;
			case OCTOTHORPE:
				if (c == '\r' || c == '\n')
				{
					state = START;
				}
				else if (inStream.eof())
				{
					state = END;
				}
				else
				{
					//state = OCTOTHORPE;
				}
				break;
			case INVALID:
				throw std::runtime_error("Invalid character found.");
				break;
			default:
				throw std::runtime_error("state invalid : " + state);
				break;
			}
		}

		image.setPath(fs::u8path(path));
		image.setTagList(tagList);
	}
	catch(std::exception& ex)
	{
		//TODO rendre les exceptions loggables
		std::cerr << ex.what() << std::endl;
		inStream.clear(std::ios::failbit);
	}

	return inStream;
}

std::ostream & operator<<(std::ostream& outStream, const Image& image)
{
	//path
	outStream << '"';
	for (const auto& c : image.getPath().u8string())
	{
		if (c == '\\')
			outStream << R"(\\)";
		else if (c == '"')
			outStream << R"(\")";
		else
			outStream << c;
	}
	outStream << '"';

	//tags
	for (const auto& tag : image.getTagList())
	{
		outStream << '"';
		for (const auto& c : tag)
		{
			if (c == '\\')
				outStream << R"(\\)";
			else if (c == '"')
				outStream << R"(\")";
			else
				outStream << c;
		}
		outStream << '"';
	}	

	outStream << ';';

	return outStream;
}
