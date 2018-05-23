#include "../Collection/FilteredCollection.hpp"
#include "save.hpp"
#include <fstream>
#include <sstream>


namespace fs = std::experimental::filesystem;

void saveCollec(const std::experimental::filesystem::path& savePath,Collection<Image>& collec)
{
    std::ofstream out(savePath);
    if(!out)
        throw std::runtime_error("Impossible de sauvegarder.");
	//TODO à gérer : quand on ferme sans taper de nom de save
    
    for(const auto& img : collec)
    {
		out << img << std::endl;
	}
}

//HACK proposition avec utilisation du parseur
//void saveCollec(const std::experimental::filesystem::path& savePath, Collection<Image>& collec)
//{
//	std::ofstream out(savePath);
//	if(!out)
//	    throw std::runtime_error("Impossible de sauvegarder");
//
//	auto parsed_vect = parse_collection_using_path(collec);
//	for (auto& parsedItem : parsed_vect)
//	{
//		out << parsedItem.first.u8string();
//		for (auto& tag : parsedItem.second)
//		{
//			out << tag.u8string();
//		}
//		out << std::endl;
//	}
//}


void updateCollec(const std::experimental::filesystem::path& loadPath, Collection<Image>& collec)
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
		if (std::getline(ss, filename, ':')) {
			TagList savedList;
			Tag tag;
			while (std::getline(ss, tag, ','))
			{
				if (tag[0] == ':') tag.erase(0);
				savedList.insert(tag);
			}
            if(!savedList.empty()) {
                fs::path path = filename;
                FilteredCollection<Image> imagesWithRigthPath(collec, [&path](const Image& img){ return img.getPath() == path;});
                for(auto& img : imagesWithRigthPath)
                    for(auto& savedTag : savedList)
                        img.getTagList().insert(savedTag);
            }
        }
    }
}
