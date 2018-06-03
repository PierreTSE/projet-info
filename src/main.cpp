﻿#include "Collection/CollectionIterator.hpp"
#include "Collection/CollectionPool.hpp"
#include "Collection/FilteredCollection.hpp"
#include "ConsolePrototype/config.hpp"
#include "ConsolePrototype/menu.hpp"
#include "ConsolePrototype/save.hpp"
#include "FileDialog/FileDialog.hpp"
#include "Image/Image.hpp"
#include "Interface/ButtonWidget.hpp"
#include "Interface/GridWidget.hpp"
#include "Interface/ListWidget.hpp"
#include "Interface/ScrollWidget.hpp"
#include "Interface/WindowWidget.hpp"
#include "Utilities/Utilities.hpp"
#include "system_target.hpp"
#include <chrono>
#include <experimental/filesystem>
#include <iostream>
#include <thread>


namespace fs = std::experimental::filesystem;
using namespace cimg_library;
using namespace std;
using img = cimg_library::CImg<unsigned char>;

int main()
{
	auto wd = fs::current_path(); //working directory

	TagList possibleTags;
	fs::path tagsPath = "tags.txt";

#ifdef WINDOWS
	if (fs::exists(tagsPath))
		auto possibleTags = loadTagList(tagsPath);
	auto collection = std::move(createPoolFromSave(R"(D:\_Télécom Saint-Etienne\_Projets\mini projet\Dossier test\00save.txt)"));
	//importFromDirectory(R"(D:\_Télécom Saint-Etienne\_Projets\\mini projet\Dossier test)", collection);
#endif
#ifndef WINDOWS
	if (fs::exists(tagsPath))
		possibleTags = loadTagList(tagsPath);
	auto collection = std::move(createPoolFromSave(getOpenFileName()));
	//importFromDirectory(browseFolder(), collection);
#endif

    GridWidget gridTest(collection, 1000, 500);
    ScrollWidget scrollTest(gridTest, { 1000, 500 });
    ButtonWidget buttonTest("clique pour savoir comment est thomas");
	const vector<string> texts = { "bouton 1","bouton 2", "bouton higfyo"," vfvz \\\"evb  azreb" };
	ListWidget listTest(texts,true);
    
    WindowWidget window(buttonTest, { 800, 800 });

	const img imageTest = listTest.render();


	CImgDisplay main_disp(imageTest.width(),imageTest.height(),"prout");
    while(!main_disp.is_closed())
    {
		main_disp.display(imageTest);
    }

	return 0;
    
    while(window.is_open())
    {
        window.manageEvents();
        window.display();
        std::this_thread::sleep_for(10ms);
    }
    
    return 0;  

    //---------------------------------------------------------------------------------------------------------------
    //menu console prototype

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
											 , nullptr
											 , CImg<>());
        switch(c)
        {
            case 0 :
                {
                    std::cout << "Quelle image ?" << std::endl;
                    auto img = choix_image(collection);
                    if(img != collection.end())
                    {
                        affTags(img->getTagList());
                    }
                }
                break;

            case 1 :
                {
                    std::cout << "Quelle image ?" << std::endl;
                    auto img = choix_image(collection);
                    if(img != collection.end())
                    {
                        std::cout << "Quel Tag ?" << std::endl;
                        auto tag = choix_tag(possibleTags);
                        if(!tag.empty())
                        {
                            img->getTagList().insert(tag);
                        }
                    }
                }
                break;

            case 2 :
                {
                    std::cout << "Quelle image ?" << std::endl;
                    auto img = choix_image(collection);
                    if(img != collection.end())
                    {
                        std::cout << "Quel Tag ?" << std::endl;
                        auto tag = choix_tag(img->getTagList());
                        if(!tag.empty())
                        {
                            img->getTagList().erase(tag);
                        }
                    }
                }
                break;

            case 3 :
                {
                    std::cout << "Quel Tag ?" << std::endl;
                    auto tag = choix_tag(possibleTags);
                    auto filtre = FilteredCollection<Image>(collection, [&tag](const Image& img)
                    {
                        return img.getTagList().find(tag) != img
                                                             .getTagList().
                                                             end();
                    });
                    listImg(filtre);
                }
                break;

            case 4 :
                {
                    Tag tag;
                    std::cout << "Saississez un tag" << std::endl;
                    std::cin >> tag;
                    possibleTags.insert(tag);
                }
                break;

            case -1 :
				{
					quit = true;
					auto savePath = getSaveFileName();
					saveCollec(savePath, collection);
				}
                break;
            default :
                break;
        }
    }
    
	fs::current_path(wd);
    saveTagList(tagsPath, possibleTags);
    
    return 0;
}