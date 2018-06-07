#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Interface/WindowWidget.hpp"
#include "Collection/CollectionPool.hpp"
#include "Interface/GridWidget.hpp"


class Application
{
    public:
        Application();
        int execute();
        
    private:
        std::unique_ptr<CollectionPool<Image>> collection_ = nullptr;
        TagList possibleTags_;
        
        WindowWidget window_;
};


#endif //APPLICATION_HPP
