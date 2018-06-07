#include "Collection/CollectionIterator.hpp"
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
#include "Interface/ImageWidget.hpp"
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
    //auto collection = std::move(createPoolFromSave(getOpenFileName()));
    auto collection = std::move(createPoolFromSave(fs::path("/home/thomas/Images/images-ppm/save.txt")));
    //importFromDirectory(browseFolder(), collection);
    importFromDirectory(fs::path("/home/thomas/Images/images-ppm/"), collection);
#endif

	cout << __cplusplus;

	string s = R"(ÉÀ}æö,¦£ éèàùç)";

    GridWidget gridTest(collection, 1000, 500, {200, 200});
    ScrollWidget scrollTest(gridTest, { 1000, 500 });
    ButtonWidget buttonTest("clique pour savoir comment est thomas");
	buttonTest.setCallBack([](ClickEvent ce, ButtonWidget* be) {return true; });
	const vector<string> texts = { "bouton 1", R"(ÉÀ}æö,¦£ éèàùç)"," vfvz \\\"evb  azreb" };
	ImageWidget imageTest(*collection.begin(), dim_t{100, 100});
	ListWidget listColumn(texts,true);
    for(size_t i = 0 ; i < listColumn.getLength() ; ++i)
    {
		listColumn.setCallBack(i);
    }
	ListWidget listLine(texts);
	for (size_t i = 0; i < listLine.getLength(); ++i)
	{
		listLine.setCallBack(i);
	}
    
    WindowWidget window1(listLine, { listLine.size().x, listLine.size().y });
	WindowWidget window2(listColumn, { listColumn.size().x, listColumn.size().y });
	//WindowWidget window(buttonTest, { buttonTest.size().x,buttonTest.size().y });
	//WindowWidget window1(scrollTest, { scrollTest.size().x, scrollTest.size().y });
    
    while(window1.is_open() || window2.is_open())
    {
        if(window1.is_open())
        {
			window1.manageEvents();
			window1.display();            
        }
		if (window2.is_open())
		{
			window2.manageEvents();
			window2.display();
		}
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