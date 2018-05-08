#ifndef MENU_HPP
#define MENU_HPP

#include "../Collection/Collection.hpp"
#include "../Image/Image.hpp"

void menu();
int choix(int max);
Collection<Image>::iterator choix_image(Collection<Image>& collec);
void affTags(const TagList& tagList);
Tag choix_tag(const TagList& tagList);
void listImg(Collection<Image>& collec);

#endif //MENU_HPP
