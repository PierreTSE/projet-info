#include "Collection/Collection.hpp"
#include "Collection/CollectionIterator.hpp"
#include "Collection/CollectionPool.hpp"
#include "Collection/FilteredCollection.hpp"
#include "ConsolePrototype/config.hpp"
#include "ConsolePrototype/menu.hpp"
#include "ConsolePrototype/save.hpp"
#include "FileDialog/FileDialog.hpp"
#include "Image/Image.hpp"
#include "system_target.hpp"
#include <chrono>
#include <exception>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <thread>

namespace fs = std::experimental::filesystem;
using namespace cimg_library;
using namespace std;
using img = cimg_library::CImg<unsigned char>;

/** @fn createPoolFromSave
/*  @brief Charge une collection d'images à partir du chemin d'un fichier de sauvegarde
/*  @param savePath Chemin du fichier de sauvegarde décrivant la collection à charger
/*  @return @c CollectionPool d'images chargée par les données contenues dans le fichier en paramètre
/*
/*  Cette fonction charge dans chaque image de la @c CollectionPool de retour les attributs
/*  path_ et tagList_ à partir du fichier de sauvegarde d'extension .txt . 
/*  Le fichier de sauvegarde doit se trouver dans le répertoire où se trouvent les images à charger.
**/
CollectionPool<Image> createPoolFromSave(const fs::path& savePath)
{
    CollectionPool<Image> collectionPool;
    
    if(fs::exists(savePath) and fs::is_regular_file(savePath) and savePath.extension() == ".txt")
    {
        std::ifstream in(savePath);
        if (!in)
            throw std::runtime_error("Impossible de lire le fichier.");

        Image image;
        while (in >> image) //charge le path_ et la tagList_ par l'automate de l'opérateur surchargé >>
        {
            image.loadImage(); //charge la CImg
            collectionPool.push_back(std::move(image));
        }
    }
    
    return collectionPool;
}

/** @fn createPoolFromDirectory
/*  @brief Charge une collection d'images à partir du chemin d'un répertoire de sauvegarde
/*  @param directoryPath Chemin du fichier de sauvegarde décrivant la collection à charger
/*  @return @c CollectionPool d'images chargée par les données contenues dans le répertoire en paramètre
/*
/*  Appelle la fonction @fn createPoolFromSave sur le fichier texte unique du répertoire en paramètre.
**/
CollectionPool<Image> createPoolFromDirectory(const fs::path& directoryPath)
{
	CollectionPool<Image> collectionPool;

	fs::path savePath;
	/* Recherche d'un fichier texte contenant la sauvegarde (chemins et tags de chaque image du dossier).
	Ce fichier doit être unique (aka la collection doit être vide). */
	for (auto& file : fs::directory_iterator(directoryPath))
	{
		if (file.path().extension() == ".txt")
		{
			if (savePath.empty())
				savePath = file.path();
			else
				throw std::runtime_error("Several save files have been found during loading.");
		}
	}

	if(savePath.empty())
		throw std::runtime_error("Not any save file have been found during loading.");
	else
		return createPoolFromSave(savePath);
}


//TODO à refondre
/** @fn getPoolFromDirectory
/*  @brief Charge une collection d'images à partir du chemin d'un répertoire de sauvegarde
/*  @param directoryPath Chemin du répertoire de sauvegarde à charger
/*  @return @c CollectionPool d'images chargée par les données contenues dans le répertoire en paramètre
/*
/*  Cette fonction charge dans chaque image de la @c CollectionPool de retour les attributs :
/*  path_ et tagList_ à partir du fichier de sauvegarde d'extension .txt
/*  qui doit être inclus dans le répertoire en paramètre où se situent les images à charger.
**/
void getPoolFromDirectory(const fs::path& directoryPath, CollectionPool<Image>& collectionPool)
{
	if (fs::is_directory(directoryPath))
	{
		fs::path savePath; // Chemin de la sauvegarde d'extension .txt

		/*
		/ Chargement de toutes les images (d'extension .ppm)
		*/
		for (auto& file : fs::directory_iterator(directoryPath))
		{
			////Images d'extension .ppm
			if (file.path().extension() == ".ppm")
			{
				Image image(file.path(), std::move(std::unique_ptr<img>(new img(file.path().u8string().c_str()))), {});
				collectionPool.push_back(std::move(image));
			}


			/* Recherche d'un fichier texte contenant la sauvegarde (chemins et tags de chaque image du dossier).
			   Ce fichier doit être unique (aka la collection doit être vide). */
			//if (file.path().extension() == ".txt")
			//{
			//	if (savePath.empty())
			//		savePath = file.path();
			//	else
			//		throw std::runtime_error("Several save files have been found during loading.");
			//}
		}

		// Chaque image est associée à ses Tags depuis le ficher de sauvegarde grâce à son attribut path.
		//if (!savePath.empty())
		//{
		//	std::ifstream in(savePath);
		//	if (!in)
		//		throw std::runtime_error("Peut pas lire");
        //
		//	Image image;
		//	while (in >> image)
		//	{
		//		image.loadImage();
		//		collectionPool.push_back(std::move(image));
		//	}
		//}
		//else
		//	throw std::runtime_error("Not any save file have been found during loading.");

	}
	else
	{
		throw std::runtime_error("No directory selected.");
	}
}

int main()
{
	//working directory
	auto wd = fs::current_path();

	TagList possibleTags;
	fs::path tagsPath = "tags.txt";
	if (fs::exists(tagsPath))
		possibleTags = loadTagList(tagsPath);

	auto collection = std::move(createPoolFromSave(getOpenFileName()));
	getPoolFromDirectory(browseFolder(), collection);
	//CollectionPool<Image> collection = getPoolFromDirectory(R"(D:\_Télécom Saint-Etienne\_Projets\mini projet\Dossier test)");


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

	while (!main_disp.is_closed())
	{
		std::this_thread::sleep_for(10ms);		

		main_disp.resize(visu).display(visu);

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