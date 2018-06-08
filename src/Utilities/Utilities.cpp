#include "Utilities.hpp"
#include <iostream>
#include <algorithm>


namespace fs = std::experimental::filesystem;
using img = cimg_library::CImg<unsigned char>;

/** @fn createPoolFromSave
/*  @brief Charge une collection d'images à partir du chemin d'un fichier de sauvegarde
/*  @param savePath Chemin du fichier de sauvegarde décrivant la collection à charger
/*  @return @c CollectionPool d'images chargée par les données contenues dans le fichier en paramètre
/*
/*  Cette fonction charge dans chaque image de la @c CollectionPool de retour les attributs
/*  path_ et tagList_ à partir du fichier de sauvegarde d'extension .txt .
/*  Le fichier de sauvegarde doit se trouver dans le répertoire où se trouvent les images à charger.
**/
CollectionPool<Image> createPoolFromSave(const fs::path& savePath)
{
	CollectionPool<Image> collectionPool;
	if (fs::exists(savePath) && fs::is_regular_file(savePath))
	{
		if (savePath.extension() == ".txt")
		{
			std::ifstream in(savePath);
			if (!in)
				throw std::runtime_error("Impossible de lire le fichier.");

			Image image;
			while (in >> image) //charge le path_ et la tagList_ par l'automate de l'opérateur surchargé >>
			{
				image.loadImage(); //charge la CImg
				collectionPool.push_back(std::move(image));
			}
		}
	}
	else
	{
		throw std::runtime_error("Path in parameter does not refer to a valid save file.");
	}
    return collectionPool;
}

/** @fn createPoolFromDirectory
/*  @brief Charge une collection d'images à partir du chemin d'un répertoire de sauvegarde
/*  @param directoryPath Chemin du fichier de sauvegarde décrivant la collection à charger
/*  @return @c CollectionPool d'images chargée par les données contenues dans le répertoire en paramètre
/*
/*  Appelle la fonction @fn createPoolFromSave sur le fichier texte unique du répertoire en paramètre.
**/
CollectionPool<Image> createPoolFromDirectory(const fs::path& directoryPath)
{
	if (!is_directory(directoryPath))
		throw std::runtime_error("Path in parameter does not refer to a valid directory.");

	CollectionPool<Image> collectionPool;

	fs::path savePath;
	/* Recherche d'un fichier texte contenant la sauvegarde (chemins et tags de chaque image du dossier).
	Ce fichier doit être unique. */
	for (auto& file : fs::directory_iterator(directoryPath))
	{
		if (file.path().extension() == ".txt")
		{
			if (savePath.empty())
				savePath = file.path();
			else
				throw std::runtime_error("Several save files have been found during loading.");
		}
	}

	if (savePath.empty())
		throw std::runtime_error("Not any save file have been found during loading.");

	return createPoolFromSave(savePath);
}

/** @fn importFromDirectory
*  @brief Ajoute une collection externe à une collection en paramètre
*  @param directoryPath Chemin du répertoire de sauvegarde où se trouve la collection à charger et ajouter
*  @param collectionPool Collection d'images à laquelle est ajoutée les images chargées du directoryPath
*
*  Cette fonction ajoute dans @param collectionPool les images au format 'ppm' contenues dans
*  le répertoire @param directoryPath .
**/
void importFromDirectory(const fs::path& directoryPath, CollectionPool<Image>& collectionPool)
{
	if (fs::is_directory(directoryPath))
	{
		// Chargement de toutes les images (d'extension .ppm)
		for (auto& file : fs::directory_iterator(directoryPath))
		{
			//Images d'extension .ppm
			if (file.path().extension() == ".ppm" && std::find_if(collectionPool.begin(), collectionPool.end(), [&file](const Image& image) {return image.getPath() == file.path(); }) == collectionPool.end())
			{
				Image image(file.path());
				image.loadImage();
				collectionPool.push_back(std::move(image));
			}
		}
	}
	else
	{
		throw std::runtime_error("Parameter is not a directory.");
	}
}

std::string UTF8toISO8859_1(const std::string& str)
{
	std::string str_out;
	for (size_t i = 0; i < str.length(); i++)
	{
		if ((static_cast<unsigned char>(str[i]) & 0x80) == 0)
			str_out.push_back(str[i]);
		else if ((static_cast<unsigned char>(str[i]) & 0xE0) == 0xC0) //caractère sur 2 octets
		{
		    unsigned int codepoint = (str[i] & 0x1F) << 6;
			if (i + 1 >= str.size())
			{
				std::cerr << "Unexpected end of string." << std::endl;
			}
			else if ((str[i + 1] & 0xC0) != 0x80)
			{
				std::cerr << "Invalid character at position " << i << " : " << std::hex << str[i] << std::endl;
			}
			else {
				codepoint += str[i + 1] & 0x3F;
				if (codepoint > 255)
					str_out.push_back('?');
				else
				{
					str_out.push_back(codepoint);
				}
			}
			i += 1;
		}
		else if ((static_cast<unsigned char>(str[i]) & 0xF0) == 0xE0)
		{
			str_out.push_back('?');
			i += 2;
		}
		else if ((static_cast<unsigned char>(str[i]) & 0xF8) == 0xF0)
		{
			str_out.push_back('?');
			i += 3;
		}
		else
		{
			std::cerr << "Invalid character at position " << i << " : " << std::hex << str[i] << std::endl;
		}
	}

	return str_out;
}


std::string ISOtoUTF8(const std::string& str)
{
	std::string out;
	for(auto& c : str)
	{
		if((static_cast<unsigned char>(c) & 0x80) == 0)
			out.push_back(c);
		else
		{
			out.push_back(0xC0 + (static_cast<unsigned char>(c) >> 6));
			out.push_back(0x80 + (static_cast<unsigned  char>(c) & 0x3F));
		}
	}
	
	return out;
}