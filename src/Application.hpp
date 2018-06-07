#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Interface/WindowWidget.hpp"
#include "Collection/CollectionPool.hpp"
#include "Interface/GridWidget.hpp"
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

		bool loadPossibleTags();
		void savePossibleTags() const;

    private:
        void update();
    
        void initialWindow();
        void collectionWindow();
    
        std::unique_ptr<CollectionPool<Image>> collection_ = nullptr;
        TagList possibleTags_;
        
        WindowWidget window_;
        std::optional<std::function<void()>> updateFunction_;
        
        std::experimental::filesystem::path savePath_;
        
        std::map<std::string, std::any> variables_;

		void actualSave() const;
};


#endif //APPLICATION_HPP
