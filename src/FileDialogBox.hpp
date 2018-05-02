#ifndef FILEDIALOGBOX_H
#define FILEDIALOGBOX_H

#include <experimental/filesystem>
#include <string>


/**
 * @brief Ouvre une boîte de dialogue qui demande un fichier à ouvrir
 * @param title titre de la boîte de dialogue
 * @param filter filtre sur les fichiers à ouvrir
 * @param initailDir Dossier dans lequel se trouvera la boîte de dialogue à l'ouverture
 * @return chemin vers le fichier sélectionné
 */
std::experimental::filesystem::path getFileDialogBox(const std::string& title, 
                                                     const std::string& filter, 
                                                     const std::experimental::filesystem::path& initailDir);

#endif