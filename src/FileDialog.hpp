#ifndef FILEDIALOG_HPP
#define FILEDIALOG_HPP

#include "system_target.hpp"
#include <string>

std::string getOpenFileName(const std::string &title = "Open a file", const std::string &filter = "All files (*.*)", const std::string &initialDir = "");
std::string getSaveFileName(const std::string &title = "Save file as", const std::string &filter = "All files (*.*)", const std::string &initialDir = "");
std::string browseFolder(const std::string &title = "Select a directory", const std::string &initialDir = "");

#endif // !FILEDIALOG_HPP
