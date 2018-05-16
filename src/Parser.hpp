#ifndef PARSER_HPP
#define PARSER_HPP

#include "Image\Image.hpp"
#include <experimental\filesystem>
#include <fstream>
#include <locale>
#include <string>
#include <utility>
#include <vector>


std::pair<std::string, std::string> tokenize(const std::string&, const std::string&);

std::vector<std::string> separate(const std::string&, const std::string&);

std::vector<std::pair<std::string, std::vector<std::string>>> unparse(const std::fstream&, std::locale);

std::vector<std::pair<std::string, std::vector<std::string>>> unparse(const std::experimental::filesystem::path&, std::locale);

#endif // !PARSER_HPP


