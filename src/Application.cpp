#include "Application.hpp"
#include "Interface/MenuBarWidget.hpp"
#include "Interface/TagSetterWidget.hpp"
#include "Utilities/Utilities.hpp"
#include "FileDialog/FileDialog.hpp"
#include "Interface/ScrollWidget.hpp"
#include "ConsolePrototype/config.hpp"
#include <chrono>
#include <thread>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
using namespace std::chrono_literals;

Application::Application() :
    window_{nullptr, {1000, 500}}
{
    // TODO ouvrir une fenêtre vide avec les boutons pour ouvrir et importer
    auto temp = createPoolFromSave(getOpenFileName());
    collection_.reset(new CollectionPool<Image>(std::move(temp)));
    //importFromDirectory(browseFolder(), *collection_);

    fs::path tagsPath = "tags.txt";
    if (fs::exists(tagsPath))
        possibleTags_ = loadTagList(tagsPath);
    
    //auto grid = new GridWidget(*collection_, 1000, 500, {150, 150});
    auto tagger = new TagSetterWidget(possibleTags_, *collection_, 1000, 500);
    auto scroll = new ScrollWidget(tagger, {1000, 500});
    window_.setContent(scroll);
}

int Application::execute()
{
    while(window_.is_open())
    {
        window_.manageEvents();
        window_.display();
        std::this_thread::sleep_for(10ms);
    }
    
    return 0;
}

bool Application::saveAs(std::experimental::filesystem::path path)
{
	return false;
}

bool Application::save(std::experimental::filesystem::path path)
{
	return false;
}
