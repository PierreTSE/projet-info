#include "ConsolePrototype/config.hpp"
#include "ConsolePrototype/menu.hpp"
#include "Collection/CollectionIterator.hpp"
#include "Collection/FilteredCollection.hpp"
#include "Collection/Collection.hpp"
#include "Collection/CollectionPool.hpp"
#include "FileDialog/FileDialog.hpp"
#include "Image/Image.hpp"
#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <exception>

namespace fs = std::experimental::filesystem;


template <typename img_t>
CollectionPool<Image<img_t>> getPoolFromDirectory()
{
	CollectionPool<Image<img_t>> collectionPool;

	fs::path directoryPath = browseFolder();
	if (fs::is_directory(directoryPath))
	{
		TagList taglist;

		for (auto& file : fs::directory_iterator(directoryPath))
		{
			if (file.path().extension() == ".txt")
			{
				std::ifstream tagListFile(file.path(), std::ios::in);
				//TODO gestion de la tagList particulière
				break;
			}
		}
		for (auto& file : fs::directory_iterator(directoryPath))
		{
			if (file.path().extension() == ".ppm")
			{
				Image<img_t> img(file.path(), nullptr, {}); //TODO gérer le fichier image, ajouter tagList
				collectionPool.push_back(std::move(img));
			}
		}
		return collectionPool;
	}
	else
	{
		throw std::runtime_error("Pas un répertoire");
	}
}


int main()
{
    TagList possibleTags;
    fs::path tagsPath = "tags.txt";
    if(fs::exists(tagsPath))
        possibleTags = loadTagList(tagsPath);
    
    CollectionPool<Image<int>> collection = getPoolFromDirectory<int>();
    
    bool quit = false;
    
    // Appeler begin depuis un pointeur ou une référence de Collection pointant sur un CollectionPool cause un segfault
    Collection<Image<int>>* ptr = &collection;
    ptr->begin();
    ptr->end();
    for(auto& img : collection)
        std::cout << img.getPath() << std::endl;
    
    while(!quit) {
        menu();
        int c = choix(5);
        switch(c)
        {
            case 1: 
            {
                std::cout << "Quelle image ?" << std::endl;
                auto img = choix_image(collection);
                if(img != collection.end()) {
                    affTags(img->getTagList());
                }
            }
            break;
                
            case 2:
            {
                std::cout << "Quelle image ?" << std::endl;
                auto img = choix_image(collection);
                if(img != collection.end()) {
                    std::cout << "Quel Tag ?" << std::endl;
                    auto tag = choix_tag(possibleTags);
                    if(!tag.empty()) {
                        img->getTagList().insert(tag);
                    }
                }
            }
            break;
            
            case 3:
            {
                std::cout << "Quel Tag ?" << std::endl;
                auto tag = choix_tag(possibleTags);
                auto filtre = FilteredCollection<Image<int>>(collection, [&tag](const Image<int>& img){ return img.getTagList().find(tag) != img.getTagList().end(); });
                listImg(filtre);
            }
            break;
            
            case 4:
            {
                Tag tag;
                std::cout << "Saississez un tag" << std::endl;
                std::getline(std::cin, tag);
                possibleTags.insert(tag);
            }
            break;
                
            case 5:
                quit = true;
                break;
        }
    }
    
    saveTagList(tagsPath, possibleTags);
    
    return 0;
}