#ifndef MENU_HPP
#define MENU_HPP

#include "../Collection/Collection.hpp"
#include "../Image/Image.hpp"

void menu();
int choix(int max);
Collection<Image<int>>::iterator choix_image(Collection<Image<int>>& collec);
void affTags(const TagList& tagList);
Tag choix_tag(const TagList& tagList);
void listImg(Collection<Image<int>>& collec);

#endif //MENU_HPP
