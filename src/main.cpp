#include "ConsolePrototype/config.hpp"
#include "Collection/CollectionIterator.hpp"
#include "Collection/FilteredCollection.hpp"
#include "Collection/Collection.hpp"
#include "Collection/CollectionPool.hpp"
#include "FileDialog/FileDialog.hpp"
#include "Image/Image.hpp"
#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


int main()
{
    TagList possibleTags;
    fs::path tagsPath = "tags.txt";
    if(fs::exists(tagsPath))
        possibleTags = loadTagList(tagsPath);
    
    CollectionPool<Image<int>> collection;
    
    bool quit = false;
    
    while(!quit) {
        std::cin.get();
        quit = true;
    }
    
    saveTagList(tagsPath, possibleTags);
    
    return 0;
}