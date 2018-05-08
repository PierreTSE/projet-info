#ifndef PARSER_HPP
#define PARSER_HPP

#include <algorithm>
#include <cctype>
#include <exception>
#include <experimental\filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class ClassBase
{
	private:
		ClassBase() = default;
};

template <typename T>
class Class : public ClassBase {
	public:
		using type_ = T;
		Class() = default;
		explicit Class(const T& value) : value_{ value } {}
		T value_;
};

std::pair<std::string, std::string> tokenize(const std::string& str, const std::string& separator)
{
	auto pos = str.find(separator);

	if (pos == std::string::npos)
		return { str, "" };
	else
		return { str.substr(0, pos), str.substr(pos + 1) };
}

std::vector<std::string> separate(const std::string& str, const std::string& separator)
{
	std::vector<std::string> vect;

	size_t begin = 0, end;
	while ((end = str.find(separator, begin)) != std::string::npos)
	{
		vect.push_back(str.substr(begin, end));
		begin = end + separator.length();
	}

	return vect;
}

std::unordered_map<std::string, std::unique_ptr<ClassBase>> parser(std::fstream& fs)
{
	std::unordered_map<std::string, std::unique_ptr<ClassBase>> map;

	fs.seekg(0);
	while (!fs.eof()) //on parcourt le flux
	{
		std::string workstr = ""; //les blocs à parser sont rassemblées en une unique string

		std::string line;
		do
		{
			std::getline(fs, line);
			if (line[0] != '#') //les commentaires (commençant par #) sont ignorés
				workstr.append(line);
		} while (line.find(";") == std::string::npos);

		workstr.erase(std::remove_if(workstr.begin(), workstr.end(), [](char c) { return !(std::isalnum(c) || std::ispunct(c)); }), workstr.end());
			//on retire tous les caractères non alphanumériques ou de ponctuation

		/* c'est la merde ! j'aimerais faire un parser qui puisse lire un truc comme ça
				GrosMachin:
					Machin1:
						truc11=...,
						truc12=...,
					Machin2:
						truc21=...,
						PetitMachin:
							truc211=...,
							truc222=...,
						truc23=...,
				;

			et ranger chaque (avec des ':') dans une map qui contient le nom à gauche du '=' en key, et la chose à droite en donnée
			et renvoyer une unique map, qui contient une map, qui contient une map, etc... pour lister toutes les listes (une liste = une map)
		*/
		
		std::vector<std::string> vect;
		std::pair<std::string, std::string> pair;

	}
}

std::unordered_map<std::string, std::unique_ptr<ClassBase>> parser(const std::experimental::filesystem::path& path)
{
	std::fstream fs;
	fs.open(path.c_str());
	if (!fs.is_open())
		throw std::runtime_error("Not found : " + path.string());
	else
		return parser(fs);
}

#endif // !PARSER_HPP

