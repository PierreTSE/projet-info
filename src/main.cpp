#include "ConsolePrototype/config.hpp"
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
			if (file.path().extension() == "txt")
			{
				std::ifstream tagListFile(file.path(), std::ios::in);
				//TODO gestion de la tagList particulière
				break;
			}
		}
		for (auto& file : fs::directory_iterator(directoryPath))
		{
			if (file.path().extension() == "ppm")
			{
				Image<img_t> img(file.path(), nullptr, {})); //TODO gérer le fichier image, ajouter tagList
				collectionPool.push_back(img);
			}
		}
	}
	else
	{
		throw std::runtime_error;
	}
}


int main()
{
    TagList possibleTags;
    fs::path tagsPath = "tags.txt";
    if(fs::exists(tagsPath))
        possibleTags = loadTagList(tagsPath);
    
    CollectionPool<Image<int>> collection;
    
    bool quit = false;
    
    while(!quit) {
        std::cin.get();
        quit = true;
    }
    
    saveTagList(tagsPath, possibleTags);
    
    return 0;
}