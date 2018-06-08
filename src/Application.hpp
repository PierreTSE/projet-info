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
#include <list>
#include <optional>


class Application
{
    public:
        Application();
		~Application();
        int execute();

        bool saveAs();
        bool save();

		bool loadPossibleTags();
		void savePossibleTags() const;

    private:
        void update();
        
        ListWidget FichierList();
    
        void initialWindow();
        void collectionWindow();
        void tagSetterWindow();
		void imageViewerWindow();
		void imageSearchWindow();

        void actualSave() const;
    
        std::unique_ptr<CollectionPool<Image>> collection_ = nullptr;
        std::optional<FilteredCollection<Image>> selected_;
        TagList possibleTags_;

		std::list<FilteredCollection<Image>> searchedColl_;
		TagList filter_;
        
        WindowWidget window_;
        std::optional<std::function<void()>> updateFunction_;
        
        std::experimental::filesystem::path savePath_;
		std::experimental::filesystem::path wd_;
        
        std::map<std::string, std::any> variables_;

		bool isSearching_ = false;
};


#endif //APPLICATION_HPP
