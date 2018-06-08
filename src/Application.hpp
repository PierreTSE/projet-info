#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Interface/WindowWidget.hpp"
#include "Collection/FilteredCollection.hpp"
#include "Collection/CollectionPool.hpp"
#include "Interface/ListWidget.hpp"
#include "Image/Image.hpp"
#include <map>
#include <string>
#include <any>
#include <optional>


class Application
{
    public:
        Application();
        int execute();

        bool saveAs();
        bool save();

    private:
        void update();
        
        ListWidget FichierList();
    
        void initialWindow();
        void collectionWindow();
        void tagSetterWindow();

        void actualSave() const;
    
        std::unique_ptr<CollectionPool<Image>> collection_ = nullptr;
        std::optional<FilteredCollection<Image>> selected_;
        TagList possibleTags_;
        
        WindowWidget window_;
        std::optional<std::function<void()>> updateFunction_;
        
        std::experimental::filesystem::path savePath_;
        
        std::map<std::string, std::any> variables_;
};


#endif //APPLICATION_HPP
