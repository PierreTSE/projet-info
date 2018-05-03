#include "config.hpp"
#include <fstream>


void saveTagList(const std::experimental::filesystem::path& saveFile, const TagList& tagList)
{
    std::ofstream out(saveFile);
    if(!out)
        throw std::runtime_error("Peut pas ouvrir fichier");
    for(auto& tag : tagList)
        out << tag << '\n';
}

TagList loadTagList(const std::experimental::filesystem::path& loadFile)
{
    TagList tagList;
    std::ifstream in(loadFile);
    if(!in)
        throw std::runtime_error("Peut pas ouvrir fichier");
    Tag t;
    while(std::getline(in, t))
        tagList.insert(t);
    
    return tagList;
}
