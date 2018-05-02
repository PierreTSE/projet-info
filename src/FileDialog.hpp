#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include "system_target.hpp"
#include <experimental/filesystem>
#include <string>


/**
 * @brief Ouvre une boîte de dialogue qui demande un fichier à ouvrir
 * @param title titre de la boîte de dialogue
 * @param filter filtre sur les fichiers à ouvrir
 * @param initialDir Dossier dans lequel se trouvera la boîte de dialogue à l'ouverture
 * @return chemin vers le fichier sélectionné
 */
std::experimental::filesystem::path getOpenFileName(const std::string& title = "Open a file",
                                                     const std::string& filter = "All files (*.*)",
                                                     const std::experimental::filesystem::path& initialDir = "");

//TODO PG à éditer avec path
std::string getSaveFileName(const std::string &title = "Save file as", const std::string &filter = "All files (*.*)", const std::string &initialDir = "");
std::string browseFolder(const std::string &title = "Select a directory", const std::string &initialDir = "");


#endif // !FILEDIALOG_HPP
