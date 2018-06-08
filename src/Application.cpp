#include "Application.hpp"
#include "Interface/MenuBarWidget.hpp"
#include "Interface/TagSetterWidget.hpp"
#include "Interface/GridWidget.hpp"
#include "Interface/LayoutWidget.hpp"
#include "Utilities/Utilities.hpp"
#include "FileDialog/FileDialog.hpp"
#include "Interface/ScrollWidget.hpp"
#include "ConsolePrototype/config.hpp"
#include <chrono>
#include <thread>
#include <experimental/filesystem>
#include <iostream>


namespace fs = std::experimental::filesystem;
using namespace std::chrono_literals;

Application::Application() :
    window_{nullptr, {1000, 500}}
{
    fs::path tagsPath = "tags.txt";
    if (fs::exists(tagsPath))
        possibleTags_ = loadTagList(tagsPath);
    
    //auto grid = new GridWidget(*collection_, 1000, 500, {150, 150});
    //auto tagger = new TagSetterWidget(possibleTags_, *collection_, 1000, 500);
    //auto scroll = new ScrollWidget(tagger, {1000, 500});
    //window_.setContent(scroll);
    initialWindow();
}

int Application::execute()
{
    while(window_.is_open())
    {
        window_.manageEvents();
        window_.display();
        update();
        std::this_thread::sleep_for(10ms);
    }
    
    return 0;
}

void Application::actualSave() const
{
    std::clog << savePath_ << std::endl;
	std::ofstream os(savePath_, std::ios::out | std::ios::trunc);
	if (collection_)
	{
		for (const auto& img : *collection_)
		{
			os << img << std::endl;
		}
	}
}

bool Application::saveAs()
{
	savePath_ = getSaveFileName("Enregistrer sous...", { "Texte",".txt" });
	if(savePath_ != fs::path(""))
	{
		actualSave();
		return true;
	}
	else return false;
}

bool Application::save()
{
	if(fs::exists(savePath_) && fs::is_regular_file(savePath_))
	{
		actualSave();
		return true;
	}
	else
	{
		return saveAs();
	}
}

void Application::initialWindow()
{
    std::unique_ptr<ListWidget> list(new ListWidget({u8" Charger ", u8" Créer "}));
    list->setCallBack(0, [this](ClickEvent ce, ButtonWidget* but)
        { // Charger function
            updateFunction_ = [this]()
            {
                fs::path savePath = getOpenFileName();
                if(!fs::exists(savePath) || !fs::is_regular_file(savePath))
                    return;
                savePath_ = savePath;
                auto temp = createPoolFromSave(savePath_);
                collection_.reset(new CollectionPool<Image>(std::move(temp)));
                if(collection_)
                    collectionWindow();
                else
                    initialWindow();
            };
            return true;
        });
    list->setCallBack(1, [this](ClickEvent ce, ButtonWidget* but)
        { 
            updateFunction_ = [this]()
            {// Créer une collection
                fs::path directoryPath = browseFolder();
                if(!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
                    return;
                collection_.reset(new CollectionPool<Image>);
    
                importFromDirectory(directoryPath, *collection_);
    
                collectionWindow();
            };
            return true;
        });
    
    auto menubar = new MenuBarWidget(nullptr, list.release(), window_.size());
    window_.setContent(menubar);
}

void Application::update()
{
    if(updateFunction_)
    {
        (*updateFunction_)();
        updateFunction_ = std::nullopt;
    }
}

void Application::collectionWindow()
{
    // Construction des différents menus
    ListWidget fichierList = std::move(FichierList());
    variables_["Fichier"] = fichierList;
    
    // Contruction fenêtre
    std::unique_ptr<ListWidget> list(new ListWidget({u8" Fichier "}));
    list->setCallBack(0, [this](ClickEvent ce, ButtonWidget* but)
    {
        ListWidget* listFichier = new ListWidget(std::any_cast<ListWidget>(variables_["Fichier"]));
        but->getWindow()->spawnRightClickMenu(listFichier, dim_t{0, but->size().y});
        
        return true;
    });
    
    // TODO Mettre un callback à grid
    auto grid = new GridWidget(*collection_, window_.size().x, window_.size().y, {150, 150});
    auto scroll = new ScrollWidget(grid, window_.size());
    auto menubar = new MenuBarWidget(scroll, list.release(), window_.size());
    window_.setContent(menubar);
}

ListWidget Application::FichierList()
{
    ListWidget fichierList({u8" Nouveau ", u8" Ouvrir... ", u8" Importer ", u8" Enregistrer ", u8" Enregistrer Sous... ", u8" Fermer "}, true);
    fichierList.setCallBack(0, [this](ClickEvent ce, ButtonWidget* but)
    { // Nouveau
        save();
        updateFunction_ = [this]()
        {
            collection_.reset(new CollectionPool<Image>);
            savePath_ = "";
            collectionWindow();
        };
        return true;
    });
    fichierList.setCallBack(1, [this](ClickEvent ce, ButtonWidget* but)
    { // Ouvrir
        save();
        updateFunction_ = [this]()
        {
            fs::path savePath = getOpenFileName();
            if(!fs::exists(savePath) || !fs::is_regular_file(savePath))
                return;
            savePath_ = savePath;
            auto temp = createPoolFromSave(savePath_);
            collection_.reset(new CollectionPool<Image>(std::move(temp)));
            collectionWindow();
        };
        return true;
    });
    fichierList.setCallBack(2, [this](ClickEvent ce, ButtonWidget* but)
    { // Importer
        updateFunction_ = [this]()
        {
            fs::path directoryPath = browseFolder();
            if(!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
                return;
            importFromDirectory(directoryPath, *collection_);
            collectionWindow();
        };
        return true;
    });
    fichierList.setCallBack(3, [this](ClickEvent ce, ButtonWidget* but)
    { // Enregistrer
        save();
        return true;
    });
    fichierList.setCallBack(4, [this](ClickEvent ce, ButtonWidget* but)
    { // Enregistrer Sous
        saveAs();
        return true;
    });
    fichierList.setCallBack(5, [this](ClickEvent ce, ButtonWidget* but)
    { // Fermer
        updateFunction_ = [this]()
        {
            save();
            collection_.reset(nullptr);
            savePath_ = "";
            initialWindow();
        };
        return true;
    });
    
    return fichierList;
}

void Application::tagSetterWindow()
{
    // Construction des différents menus
    ListWidget fichierList = std::move(FichierList());
    variables_["Fichier"] = fichierList;
    selected_ = FilteredCollection<Image>(*collection_, [](const Image& image) { return image.isSelected(); });

    // Contruction fenêtre
    std::unique_ptr<ListWidget> list(new ListWidget({u8" Fichier "}));
    list->setCallBack(0, [this](ClickEvent ce, ButtonWidget* but)
    {
        ListWidget* listFichier = new ListWidget(std::any_cast<ListWidget>(variables_["Fichier"]));
        but->getWindow()->spawnRightClickMenu(listFichier, dim_t{0, but->size().y});

        return true;
    });

    // TODO Mettre un callback à grid
    auto grid = new GridWidget(*selected_, window_.size().x, window_.size().y, {150, 150});
    std::unique_ptr<ScrollWidget> scroll(new ScrollWidget(grid, window_.size()));
    auto tagsetter = new TagSetterWidget(possibleTags_, *selected_, window_.size().x, window_.size().y);
    auto scroll2 = new ScrollWidget(tagsetter, window_.size());
    auto layout = new LayoutWidget(scroll.release(), scroll2, 2.0/3.0, window_.size());
    
    auto menubar = new MenuBarWidget(layout, list.release(), window_.size());
    window_.setContent(menubar);
}
