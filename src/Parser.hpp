#ifndef PARSER_HPP
#define PARSER_HPP

#include "Image\Image.hpp"
#include "Collection\Collection.hpp"
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

std::vector<std::pair<std::string, std::vector<std::string>>> parse_collection(Collection<Image>& collection);

std::vector<std::pair<std::experimental::filesystem::path, std::vector<std::experimental::filesystem::path>>> parse_collection_using_path(Collection<Image>& collection);

#endif // !PARSER_HPP


