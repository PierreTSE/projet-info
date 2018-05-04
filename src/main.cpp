#include "ConsolePrototype/config.hpp"
#include "ConsolePrototype/menu.hpp"
#include "Collection/CollectionIterator.hpp"
#include "Collection/FilteredCollection.hpp"
#include "Collection/Collection.hpp"
#include "Collection/CollectionPool.hpp"
#include "FileDialog/FileDialog.hpp"
#include "Image/Image.hpp"
#include "ConsolePrototype/save.hpp"
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
			if (file.path().extension() == ".ppm")
			{
				Image<img_t> img(file.path(), nullptr, {}); //TODO gérer le fichier image, ajouter tagList
				collectionPool.push_back(std::move(img));
			}
		}
        for (auto& file : fs::directory_iterator(directoryPath))
        {
            if (file.path().extension() == ".txt")
            {
                updateCollec(file.path(), collectionPool);
                break;
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
	auto wd = fs::current_path();

    TagList possibleTags;
    fs::path tagsPath = "tags.txt";
    if(fs::exists(tagsPath))
        possibleTags = loadTagList(tagsPath);
    
    CollectionPool<Image<int>> collection = getPoolFromDirectory<int>();
    
    bool quit = false;
    
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
                std::cout << "Quelle image ?" << std::endl;
                auto img = choix_image(collection);
                if(img != collection.end()) {
                    std::cout << "Quel Tag ?" << std::endl;
                    auto tag = choix_tag(img->getTagList());
                    if(!tag.empty()) {
                        img->getTagList().erase(tag);
                    }
                }
            }
            break;
            
            case 4:
            {
                std::cout << "Quel Tag ?" << std::endl;
                auto tag = choix_tag(possibleTags);
                auto filtre = FilteredCollection<Image<int>>(collection, [&tag](const Image<int>& img){ return img.getTagList().find(tag) != img.getTagList().end(); });
                listImg(filtre);
            }
            break;
            
            case 5:
            {
                Tag tag;
                std::cout << "Saississez un tag" << std::endl;
                std::cin >> tag;
                possibleTags.insert(tag);
            }
            break;
                
            case 6:
                quit = true;
                auto savePath = getSaveFileName();
                saveCollec(savePath, collection);
                break;
        }
    }
    
	fs::current_path(wd);
    saveTagList(tagsPath, possibleTags);
    
    return 0;
}