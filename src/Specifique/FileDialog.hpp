﻿#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <experimental/filesystem>
#include <string>


struct file_filter
{
	std::string title, pattern;
};

/**
 * \brief Ouvre une boîte de dialogue qui demande un fichier à ouvrir
 * \param title Titre de la boîte de dialogue
 * \param filter Filtre sur les fichiers à ouvrir
 * \param initialDir Chemin du dossier dans lequel se trouvera la boîte de dialogue à l'ouverture
 * \return chemin vers le fichier sélectionné
 */
std::experimental::filesystem::path getOpenFileName(const std::string& title = "Ouvrir une collection",
                                                    const file_filter& filter = { "Texte", "*.txt" },
                                                    const std::experimental::filesystem::path& initialDir = "");

/**
* \brief Ouvre une boîte de dialogue qui demande un nom de sauvegarde pour un fichier
* \param title Titre de la boîte de dialogue
* \param filter Filtre sur les fichiers à ouvrir
* \param initialDir Chemin du dossier dans lequel se trouvera la boîte de dialogue à l'ouverture
* \return chemin vers le fichier sélectionné
*/
std::experimental::filesystem::path getSaveFileName(const std::string& title = "Enregistrer sous",
													const file_filter& filter = { "Texte", "*.txt" },
													const std::experimental::filesystem::path& initialDir = "");

/**
* \brief Ouvre une boîte de dialogue qui demande un répertoire à ouvrir
* \param title Titre de la boîte de dialogue
* \param initialDir Chemin du dossier dans lequel se trouvera la boîte de dialogue à l'ouverture
* \return chemin vers le répertoire sélectionné
*/
std::experimental::filesystem::path browseFolder(const std::string &title = "Importer depuis un répertoire",
												 const std::experimental::filesystem::path& initialDir = "");


#endif // !FILEDIALOG_HPP
