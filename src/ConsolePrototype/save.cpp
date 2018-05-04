#include "save.hpp"
#include "../Collection/FilteredCollection.hpp"
#include <fstream>


namespace fs = std::experimental::filesystem;

void saveCollec(const std::experimental::filesystem::path& savePath, Collection<Image<int>>& collec)
{
    std::ofstream out(savePath);
    if(!out)
        throw std::runtime_error("Peut pas sauver");
    
    for(auto& img : collec)
    {
        out << img.getPath().u8string() << ":";
        for(auto& tag : img.getTagList())
            out << tag << ',';
        out << std::endl;
    }
}

void updateCollec(const std::experimental::filesystem::path& loadPath, Collection<Image<int>>& collec)
{
    std::ifstream in(loadPath);
    if(!in)
        throw std::runtime_error("Peut pas lire");
    
    std::string line;
    while(std::getline(in, line))
    {
        std::stringstream ss(line);
        
        // get filename
        std::string filename;
        if(std::getline(ss, filename, ':')) {
            TagList savedList;
            Tag tag;
            while(std::getline(ss, tag, ','))
                savedList.insert(tag);
            if(!savedList.empty()) {
                fs::path path = filename;
                FilteredCollection<Image<int>> imagesWithRigthPath(collec, [&path](const Image<int>& img){ return img.getPath() == path;});
                for(auto& img : imagesWithRigthPath)
                    for(auto& savedTag : savedList)
                        img.getTagList().insert(savedTag);
            }
        }
    }
}
