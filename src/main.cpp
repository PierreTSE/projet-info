#include "Collection/CollectionIterator.hpp"
#include "Collection/CollectionPool.hpp"
#include "Collection/FilteredCollection.hpp"
#include "ConsolePrototype/config.hpp"
#include "ConsolePrototype/menu.hpp"
#include "ConsolePrototype/save.hpp"
#include "FileDialog/FileDialog.hpp"
#include "Image/Image.hpp"
#include "Interface/ListWidget.hpp"
#include "Interface/ScrollWidget.hpp"
#include "Utilities/Utilities.hpp"
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
    // tests des const_iterator
    //CollectionPool<Image> collection2 = std::move(createPoolFromDirectory(R"(D:\_Télécom Saint-Etienne\_Projets\mini projet\Dossier test)"));
    ////const auto& ref = collection2;
    //const Collection<Image>& ref = collection2;
    //const Collection<Image>* ptr = &collection2;
    //const FilteredCollection<Image> fcollec(collection2, [](const Image& img) {return img.getImgPtr()->height()>img.getImgPtr()->width(); });
    //int i = 0;
    //for(const auto& img : ref)
    //{
		//cout << "ref" << i << endl; ++i;
    //}
    //i = 0;
    //for (const auto& img : *ptr)
    //{
		//cout << "ptr" << i << endl; ++i;
    //}
    //i = 0;
    //for (const auto& img : fcollec)
    //{
		//cout << "fcollec" << i << endl; ++i;
    //}

	
	//working directory
	auto wd = fs::current_path();

	TagList possibleTags;
	fs::path tagsPath = "tags.txt";
	if (fs::exists(tagsPath))
		possibleTags = loadTagList(tagsPath);

	auto collection = std::move(createPoolFromSave(getOpenFileName()));
	importFromDirectory(browseFolder(), collection);

	

	/*

	// tests parse

	std::ifstream saveStream(wd / "save.txt");

	//string str;
	//char c;
	//while (!saveStream.eof())
	//{
	//	c = saveStream.get();
	//	str += c;
	//}

	//std::string str;
	//std::getline(saveStream, str, ';');
	//std::cout << str << std::endl;
	//std::getline(saveStream, str, ';');
	//std::cout << str << std::endl;
	//std::getline(saveStream, str, ';');
	//std::cout << str << std::endl;


	Image i1, i2, i3, i4, i5;

	saveStream >> i1 >> i2 >> i3 >> i4 >> i5;

	return 0;
	//!tests parse
	
	//test affichage simpliste
	*/
	CImgList<unsigned char> list;
	for (auto& img : collection)
		list.push_back(*img.getImgPtr());
	
	//// img est une CImg<unsigned char>
	//img windowImg(800U, 400U, 1U, 3U);
	//windowImg.draw_image(10U, 10U, list[0]); //list est un vector de img
	//img temp_img(list[0]);
	//temp_img.resize(300U, 300U);
	//windowImg.draw_image(300U, 10U, temp_img);

	//CImgDisplay my_main_disp(windowImg.width(), windowImg.height(), "204*306",true);

	////while(!my_main_disp.is_closed())
	////	my_main_disp.display(windowImg);


	//my_main_disp.resize(204, 306);
	////HACK rapport (256/204=1.2549,384/306=1.2549)

	//CImgDisplay disp2(256, 384, "256*384");

	//while (1)
	//{
	//	my_main_disp.display(list[0]);
	//	disp2.display(list[0]);
	//}

	//return 0;

	//while (!my_main_disp.is_closed())
	//{
	//	std::this_thread::sleep_for(100ms);

	//	if (my_main_disp.is_resized())
	//	{
	//		my_main_disp.resize(false).display(windowImg);
	//		cout << "resized widht =" << my_main_disp.width() << " height = " << my_main_disp.height() << endl;

	//		
	//		//windowImg = windowImg_copy;
	//		//windowImg.resize(my_main_disp);
	//		//my_main_disp.display(windowImg);
	//	}

	//	if (my_main_disp.is_keyR())
	//	{
	//		my_main_disp.resize(1920,1080);
	//		my_main_disp.toggle_fullscreen(false).display(windowImg);
	//	}



	//}

	//return 0;
	

	//! test affichage simpliste

	//TODO POURQUOI les images sont plus grandes ??

	//test affichage une ligne automatique

	const unsigned int window_height = 1080U;
	const unsigned int window_width = 1920U;

	CImgDisplay main_disp(window_width,window_height,"titre super bien");

	static const double magick_ratio = 100 * 306 / 384;

	img temp_img(0U,0,1,3U);
	for (size_t i = 0; i < 5; i++)
	{
		temp_img.append(list[i].resize(-magick_ratio,-magick_ratio),'x',0.5f);
	}

	img visu(1920, temp_img.height());
	//visu.fill('0','128','255','128','0');
	visu.fill(0);
	visu.draw_image(151,0,temp_img);
	
	ListWidget listTest(collection, 1000, 500);
	ScrollWidget scrollTest(listTest, {1000, 500});
	
	auto render = scrollTest.render();
	
	main_disp.resize(1000, 500);
    main_disp.display(render);
	
	int wheel = main_disp.wheel();

	while (!main_disp.is_closed())
	{
		std::this_thread::sleep_for(10ms);		
		if(main_disp.is_resized()) {
            main_disp.resize(false);
            scrollTest.resize({main_disp.width(), main_disp.height()});
            std::cerr << "resize : " << main_disp.width() << ',' << main_disp.height() << std::endl;
        }
        
        if(wheel != main_disp.wheel())
        {
            if(main_disp.is_keyCTRLLEFT() || main_disp.is_keyCTRLRIGHT())
            {
                // Zoom event
                ZoomEvent e{main_disp.wheel() - wheel};
                wheel = main_disp.wheel();
                Event ev;
                ev.first = dim_t{main_disp.mouse_x(), main_disp.mouse_y()};
                ev.second = e;
                scrollTest.propagateEvent(ev);
            }
            else
            {
                // Scroll event
                ScrollEvent e{main_disp.wheel() - wheel};
                wheel = main_disp.wheel();
                Event ev;
                ev.first = dim_t{main_disp.mouse_x(), main_disp.mouse_y()};
                ev.second = e;
                scrollTest.propagateEvent(ev);
            }
        }
        main_disp.display(scrollTest.render());

		//main_disp.resize(render).display(render);

	}	

	return 0;

	

	
    
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