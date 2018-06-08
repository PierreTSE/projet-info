#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "../Collection/CollectionPool.hpp"
#include "../Image/Image.hpp"

/** @fn createPoolFromSave
 *  @brief Charge une collection d'images à partir du chemin d'un fichier de sauvegarde
 *  @param savePath Chemin du fichier de sauvegarde décrivant la collection à charger
 *  @return @c CollectionPool d'images chargée par les données contenues dans le fichier en paramètre
 *
 *  Cette fonction charge dans chaque image de la @c CollectionPool de retour les attributs
 *  path_ et tagList_ à partir du fichier de sauvegarde d'extension .txt .
 *  Le fichier de sauvegarde doit se trouver dans le répertoire où se trouvent les images à charger.
 */
CollectionPool<Image> createPoolFromSave(const std::experimental::filesystem::path& savePath);

/** @fn createPoolFromDirectory
 *  @brief Charge une collection d'images à partir du chemin d'un répertoire de sauvegarde
 *  @param directoryPath Chemin du fichier de sauvegarde décrivant la collection à charger
 *  @return @c CollectionPool d'images chargée par les données contenues dans le répertoire en paramètre
 *
 *  Appelle la fonction @fn createPoolFromSave sur le fichier texte unique du répertoire en paramètre.
 */
CollectionPool<Image> createPoolFromDirectory(const std::experimental::filesystem::path& directoryPath);

/** @fn importFromDirectory
 *  @brief Ajoute une collection externe à une collection en paramètre
 *  @param directoryPath Chemin du répertoire de sauvegarde où se trouve la collection à charger et ajouter
 *  @param collectionPool Collection d'images à laquelle est ajoutée les images chargées du directoryPath
 *
 *  Cette fonction ajoute dans @param collectionPool les images au format 'ppm' contenues dans
 *  le répertoire @param directoryPath .
 */
void importFromDirectory(const std::experimental::filesystem::path& directoryPath, CollectionPool<Image>& collectionPool);

/**
 * \brief Convertit de l'UTF-8 vers l'ISO-8859-1
 * \param str String encodée en UTF-8
 * \return String encodée en ISO-8859-1
 * 
 * Remplace les caractères non représentables en ISO-8859-1 par '?'.
 */
std::string UTF8toISO8859_1(const std::string& str);


std::string IOStoUTF8(const std::string& str);

#endif // UTILITIES_HPP
