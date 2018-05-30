#include "Utilities.hpp"

namespace fs = std::experimental::filesystem;
using img = cimg_library::CImg<unsigned char>;

/** @fn createPoolFromSave
/*  @brief Charge une collection d'images � partir du chemin d'un fichier de sauvegarde
/*  @param savePath Chemin du fichier de sauvegarde d�crivant la collection � charger
/*  @return @c CollectionPool d'images charg�e par les donn�es contenues dans le fichier en param�tre
/*
/*  Cette fonction charge dans chaque image de la @c CollectionPool de retour les attributs
/*  path_ et tagList_ � partir du fichier de sauvegarde d'extension .txt .
/*  Le fichier de sauvegarde doit se trouver dans le r�pertoire o� se trouvent les images � charger.
**/
CollectionPool<Image> createPoolFromSave(const fs::path& savePath)
{
	CollectionPool<Image> collectionPool;

	if (fs::exists(savePath) && fs::is_regular_file(savePath) && savePath.extension() == ".txt")
	{
		std::ifstream in(savePath);
		if (!in)
			throw std::runtime_error("Impossible de lire le fichier.");

		Image image;
		while (in >> image) //charge le path_ et la tagList_ par l'automate de l'op�rateur surcharg� >>
		{
			image.loadImage(); //charge la CImg
			collectionPool.push_back(std::move(image));
		}
	}

	return collectionPool;
}

/** @fn createPoolFromDirectory
/*  @brief Charge une collection d'images � partir du chemin d'un r�pertoire de sauvegarde
/*  @param directoryPath Chemin du fichier de sauvegarde d�crivant la collection � charger
/*  @return @c CollectionPool d'images charg�e par les donn�es contenues dans le r�pertoire en param�tre
/*
/*  Appelle la fonction @fn createPoolFromSave sur le fichier texte unique du r�pertoire en param�tre.
**/
CollectionPool<Image> createPoolFromDirectory(const fs::path& directoryPath)
{
	if (!is_directory(directoryPath))
		throw std::runtime_error("Parameter is not a directory.");

	CollectionPool<Image> collectionPool;

	fs::path savePath;
	/* Recherche d'un fichier texte contenant la sauvegarde (chemins et tags de chaque image du dossier).
	Ce fichier doit �tre unique. */
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

	if (savePath.empty())
		throw std::runtime_error("Not any save file have been found during loading.");

	return createPoolFromSave(savePath);
}

/** @fn importFromDirectory
*  @brief Ajoute une collection externe � une collection en param�tre
*  @param directoryPath Chemin du r�pertoire de sauvegarde o� se trouve la collection � charger et ajouter
*  @param collectionPool Collection d'images � laquelle est ajout�e les images charg�es du directoryPath
*
*  Cette fonction ajoute dans @param collectionPool les images au format 'ppm' contenues dans
*  le r�pertoire @param directoryPath .
**/
void importFromDirectory(const fs::path& directoryPath, CollectionPool<Image>& collectionPool)
{
	if (fs::is_directory(directoryPath))
	{
		// Chargement de toutes les images (d'extension .ppm)
		for (auto& file : fs::directory_iterator(directoryPath))
		{
			//Images d'extension .ppm
			if (file.path().extension() == ".ppm")
			{
				Image image(file.path(), std::move(std::make_unique<img>(file.path().u8string().c_str())), {});
				collectionPool.push_back(std::move(image));
			}
		}
	}
	else
	{
		throw std::runtime_error("Parameter is not a directory.");
	}
}