#ifndef SAVE_HPP
#define SAVE_HPP

#include "../Collection/Collection.hpp"
#include "../Image/Image.hpp"
#include <experimental/filesystem>


void saveCollec(const std::experimental::filesystem::path& savePath, const Collection<Image>& collec);
void updateCollec(const std::experimental::filesystem::path& loadPath, const Collection<Image>& collec);

#endif //SAVE_HPP
