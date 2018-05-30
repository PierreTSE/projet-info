#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "../Collection/CollectionPool.hpp"
#include "../Image/Image.hpp"

/** @fn createPoolFromSave
 *  @brief Charge une collection d'images � partir du chemin d'un fichier de sauvegarde
 *  @param savePath Chemin du fichier de sauvegarde d�crivant la collection � charger
 *  @return @c CollectionPool d'images charg�e par les donn�es contenues dans le fichier en param�tre
 *
 *  Cette fonction charge dans chaque image de la @c CollectionPool de retour les attributs
 *  path_ et tagList_ � partir du fichier de sauvegarde d'extension .txt .
 *  Le fichier de sauvegarde doit se trouver dans le r�pertoire o� se trouvent les images � charger.
 */
CollectionPool<Image> createPoolFromSave(const std::experimental::filesystem::path& savePath);

/** @fn createPoolFromDirectory
 *  @brief Charge une collection d'images � partir du chemin d'un r�pertoire de sauvegarde
 *  @param directoryPath Chemin du fichier de sauvegarde d�crivant la collection � charger
 *  @return @c CollectionPool d'images charg�e par les donn�es contenues dans le r�pertoire en param�tre
 *
 *  Appelle la fonction @fn createPoolFromSave sur le fichier texte unique du r�pertoire en param�tre.
 */
CollectionPool<Image> createPoolFromDirectory(const std::experimental::filesystem::path& directoryPath);

/** @fn importFromDirectory
 *  @brief Ajoute une collection externe � une collection en param�tre
 *  @param directoryPath Chemin du r�pertoire de sauvegarde o� se trouve la collection � charger et ajouter
 *  @param collectionPool Collection d'images � laquelle est ajout�e les images charg�es du directoryPath
 *
 *  Cette fonction ajoute dans @param collectionPool les images au format 'ppm' contenues dans
 *  le r�pertoire @param directoryPath .
 */
void importFromDirectory(const std::experimental::filesystem::path& directoryPath, CollectionPool<Image>& collectionPool);

#endif // UTILITIES_HPP
