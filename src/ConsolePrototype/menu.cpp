#include "menu.hpp"
#include <iostream>


void menu()
{
    std::cout << "Que faire ?" << std::endl 
              << "1) Afficher les tags d'une image" << std::endl 
              << "2) Ajouter un tag à une image" << std::endl
              << "3) Rechercher par tag" << std::endl 
              << "4) Ajouter un tag à la liste" << std::endl
              << "5) Quitter" << std::endl;
}

int choix(int max)
{
    std::cout << "Votre choix: ";
    int c;
    std::cin >> c;
    while(std::cin.fail() or c == 0 or c > max)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Saisie invalide, recommencer: ";
        std::cin >> c;
    }
    
    return c;
}

Collection<Image<int>>::iterator choix_image(Collection<Image<int>>& collec)
{
    size_t i = 0;
    for(auto& img : collec)
    {
        ++i;
        std::cout << i << ") " << img.getPath().u8string() << std::endl;
    }
    if(i == 0)
    {
        std::cout << "Pas d'images" << std::endl;
        return collec.end();
    }
    else
    {
        auto first = collec.begin();
        std::advance(first, choix(i)-1);
        return first;
    }
}

void affTags(const TagList& tagList)
{
    std::cout << '{';
    auto it = tagList.begin();
    if(it != tagList.end())
        std::cout << *it;
    for(++it ;it != tagList.end(); ++it)
        std::cout << ", " << *it;
    std::cout << '}' << std::endl;
}

Tag choix_tag(const TagList& tagList)
{
    std::cout << "Choississez un Tag" << std::endl;
    size_t i = 0;
    
    for(auto& tag : tagList)
    {
        std::cout << ++i << ") " << tag << std::endl;
    }
    if(i == 0)
    {
        std::cout << "Pas de Tags" << std::endl;
        return Tag{};
    }
    else
    {
        auto first = tagList.begin();
        std::advance(first, choix(i)-1);
        return *first;
    }
}

void listImg(Collection<Image<int>>& collec)
{
    for(auto& img : collec)
        std::cout << img.getPath().u8string() << std::endl;
}
