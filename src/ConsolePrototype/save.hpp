#ifndef SAVE_HPP
#define SAVE_HPP

#include "../Collection/Collection.hpp"
#include "../Image/Image.hpp"
#include <experimental/filesystem>


void saveCollec(const std::experimental::filesystem::path& savePath, Collection<Image<int>>& collec);
void updateCollec(const std::experimental::filesystem::path& loadPath, Collection<Image<int>>& collec);

#endif //SAVE_HPP
