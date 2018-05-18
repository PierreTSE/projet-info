#include "Collection/Collection.hpp"
#include "Collection/CollectionIterator.hpp"
#include "Collection/CollectionPool.hpp"
#include "Collection/FilteredCollection.hpp"
#include "ConsolePrototype/config.hpp"
#include "ConsolePrototype/menu.hpp"
#include "ConsolePrototype/save.hpp"
#include "FileDialog/FileDialog.hpp"
#include "Image/Image.hpp"
#include "Parser.hpp"
#include "system_target.hpp"
#include <exception>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::experimental::filesystem;
using namespace cimg_library;
using namespace std;
using img = cimg_library::CImg<unsigned char>;


CollectionPool<Image> getPoolFromDirectory(fs::path directoryPath)
{
	CollectionPool<Image> collectionPool;

	if (fs::is_directory(directoryPath))
	{
		TagList taglist;

		for (auto& file : fs::directory_iterator(directoryPath))
		{
			if (file.path().extension() == ".ppm")
			{
				Image img(file.path(), nullptr, {}); //TODO gérer le fichier image, ajouter tagList
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
		throw std::runtime_error("Pas de répertoire sélectionné");
	}
}

int main()
{
	//working directory
	auto wd = fs::current_path();


	//tests
#ifdef WINDOWS


	std::locale l;
	std::locale loc(l, new std::codecvt_utf8<wchar_t>);
#endif // WINDOWS
#ifdef LINUX
	std::locale loc;
#endif
	

	std::vector<std::pair<std::string, std::vector<std::string>>> vect_test = unparse(wd/="save.txt", loc);
	//le premier élément (string) de la paire correspond au chemin, le deuxième élément de la paire est la liste des tags (strings)

    TagList possibleTags;
    fs::path tagsPath = "tags.txt";
    if(fs::exists(tagsPath))
        possibleTags = loadTagList(tagsPath);
    
    CollectionPool<Image> collection = getPoolFromDirectory(browseFolder());
    
    bool quit = false;
    
    while(!quit) {
        //menu();
        //int c = choix(6);
		int c = cimg::dialog("Menu principal", "Choisissez l'action à effectuer."
											 , "Voir tag d'une image"
											 , "Tagger une image"
											 , "Détagger une image"
											 , "Rechercher par tag"
											 , "Ajouter à la liste"
											 , 0
											 , CImg<>());
		switch(c)
        {
            case 0: 
            {
                std::cout << "Quelle image ?" << std::endl;
                auto img = choix_image(collection);
                if(img != collection.end()) {
                    affTags(img->getTagList());
                }
            }
            break;

            case 1:
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

            case 2:
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
            
            case 3:
            {
                std::cout << "Quel Tag ?" << std::endl;
                auto tag = choix_tag(possibleTags);
                auto filtre = FilteredCollection<Image>(collection, [&tag](const Image& img){ return img.getTagList().find(tag) != img.getTagList().end(); });
                listImg(filtre);
            }
            break;
            
            case 4:
            {
                Tag tag;
                std::cout << "Saississez un tag" << std::endl;
                std::cin >> tag;
                possibleTags.insert(tag);
            }
            break;
                
            case -1:
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