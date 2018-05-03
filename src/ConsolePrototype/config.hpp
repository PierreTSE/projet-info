#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../Image/TagList.hpp"
#include <experimental/filesystem>


void saveTagList(const std::experimental::filesystem::path& saveFile, const TagList& tagList);
TagList loadTagList(const std::experimental::filesystem::path& loadFile);

#endif //CONFIG_HPP
