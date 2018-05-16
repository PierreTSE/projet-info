#include "Parser.hpp"
#include <exception>

std::pair<std::string, std::string> tokenize(const std::string& str, const std::string& separator)
{
	auto pos = str.find(separator);

	if (pos == std::string::npos)
		return { str, "" };
	else
		return { str.substr(0, pos), str.substr(pos + separator.length()) };
}

std::vector<std::string> separate(const std::string& str, const std::string& separator)
{
	std::vector<std::string> vect;

	int size = str.length();

	size_t begin = 0, end = 0;
	while (end != std::string::npos)
	{
		end = str.find(separator, begin);
		if (!str.substr(begin, end - begin).empty())
			vect.push_back(str.substr(begin, end - begin));
		begin = end + separator.length();
	} 
	//if (vect.empty() && !str.empty() && str != separator)
	//	vect.push_back(str.substr(begin, end));

	return vect;
}

std::vector<std::pair<std::string, std::vector<std::string>>> unparse(std::fstream& fs, std::locale locale)
{
	std::vector <std::pair<std::string, std::vector<std::string>>> vect_out;

	fs.seekg(0);
	while (!fs.eof()) //on parcourt le flux
	{
		std::pair<std::string, std::vector<std::string>> pair_out;

		std::string workstr = ""; //les blocs à parser sont rassemblés en une unique string

		std::string line;
		do
		{
			std::getline(fs, line);
			if (line[0] != '#') //les commentaires (commençant par #) sont ignorés
				workstr.append(line);
		} while (line.find(";") == std::string::npos && !fs.eof());//les blocs à parser terminent par ";"

		workstr = tokenize(workstr, ";").first; //on retire ce qui se trouve après ";" inclus
		workstr.erase(std::remove_if(workstr.begin(), workstr.end(), [locale](char c) { return !(c != ';' && (std::isalnum(c, locale) || ispunct(c, locale))); }), workstr.end());
			//on retire tous les caractères sauf alphanumériques ou de ponctutation excepté ";"
		
		std::pair<std::string, std::string> pair = tokenize(workstr, "::"); //on sépare selon "::"
		//std::vector<std::string> argumentList = separate(pair.second,","); //on sépare la liste après "::" selon ","

		if(!pair.first.empty())
			vect_out.emplace_back(pair.first, separate(pair.second,","));
	}

	return vect_out;
}

std::vector<std::pair<std::string, std::vector<std::string>>> unparse(const std::experimental::filesystem::path& path, std::locale locale)
{
	std::fstream fs;
	fs.open(path.c_str());
	if (!fs.is_open())
		throw std::runtime_error("Not found : " + path.string());
	else
		return unparse(fs,locale);
}
