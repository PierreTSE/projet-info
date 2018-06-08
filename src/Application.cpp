#include "Application.hpp"
#include "Interface/MenuBarWidget.hpp"
#include "Interface/TagSetterWidget.hpp"
#include "Interface/TagViewerWidget.hpp"
#include "Interface/GridWidget.hpp"
#include "Interface/LayoutWidget.hpp"
#include "Utilities/Utilities.hpp"
#include "Specifique/FileDialog.hpp"
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
    if(!loadPossibleTags())
        std::clog << "No tag list saved." << std::endl;
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

bool Application::loadPossibleTags()
{
	const fs::path tagListPath = fs::current_path()/"tags.txt";
	std::ifstream in(tagListPath);
	if (!in.is_open())
	{
		return false;
	}
	else
	{
		Tag t;
		while (std::getline(in, t))
			possibleTags_.insert(t);
		return true;
	}
}

void Application::savePossibleTags() const
{
	std::ofstream out(fs::current_path()/"tags.txt", std::ios::out | std::ios::trunc);
	for (auto& tag : possibleTags_)
		out << tag << std::endl;
}

void Application::update()
{
    if(updateFunction_)
    {
        (*updateFunction_)();
        updateFunction_ = std::nullopt;
    }
}

ListWidget Application::FichierList()
{
	ListWidget fichierList({ u8" Nouveau ", u8" Ouvrir... ", u8" Importer ", u8" Enregistrer ", u8" Enregistrer Sous... ", u8" Fermer " }, true);
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
			if (!fs::exists(savePath) || !fs::is_regular_file(savePath))
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
			if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
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

void Application::initialWindow()
{
	std::unique_ptr<ListWidget> list(new ListWidget({ u8" Charger ", u8" Cr\u00E9er " }));
	list->setCallBack(0, [this](ClickEvent ce, ButtonWidget* but)
	{ // Charger function
		updateFunction_ = [this]()
		{
			fs::path savePath = getOpenFileName();
			if (!fs::exists(savePath) || !fs::is_regular_file(savePath))
				return;
			savePath_ = savePath;
			auto temp = createPoolFromSave(savePath_);
			collection_.reset(new CollectionPool<Image>(std::move(temp)));
			if (collection_)
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
			if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
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
    
    ListWidget rightClickOnImage({u8" Afficher Image ", u8" Modifier les tags des images s\u00E9lectionn\u00E9es ", u8" Enlever les images s\u00E9lectionn\u00E9es "}, true);
    rightClickOnImage.setCallBack(0, [this](ClickEvent, ButtonWidget*)
        { // Afficher l'image
			variables_["ImageViewed"] = &std::any_cast<ImageWidget*>(variables_["imageWidget"])->getImage();
			ImageViewerWindow();            
            return true;
        });
    rightClickOnImage.setCallBack(1, [this](ClickEvent, ButtonWidget*)
        { // Modifier les tags
            updateFunction_ = [this]()
            {// Ouvrir la fenêtre tagger
                tagSetterWindow();
            };
            return true;
        });
    variables_["rightClickOnImage"] = rightClickOnImage;
    
    auto grid = new GridWidget(*collection_, window_.size().x, window_.size().y, {150, 150}, [this](ClickEvent ce, ImageWidget* iw)
        {
            if(ce.type == ClickEvent::RIGHT)
            {
                iw->getWindow()->spawnRightClickMenu(new ListWidget(std::any_cast<ListWidget>(variables_["rightClickOnImage"])));
                variables_["imageWidget"] = iw;
                return true;
            }
            return false;
        });
    auto scroll = new ScrollWidget(grid, window_.size());
    auto menubar = new MenuBarWidget(scroll, list.release(), window_.size());
    window_.setContent(menubar);
}

void Application::tagSetterWindow()
{
    // Construction des différents menus
    ListWidget fichierList = std::move(FichierList());
    variables_["Fichier"] = fichierList;
    selected_ = FilteredCollection<Image>(*collection_, [](const Image& image) { return image.isSelected(); });

    ListWidget rightClickOnImage({u8" Afficher Image ", u8" Modifier les tags des images s\u00E9lectionn\u00E9es ", u8" Enlever les images s\u00E9lectionn\u00E9es "}, true);
    rightClickOnImage.setCallBack(0, [this](ClickEvent, ButtonWidget*)
    { // Afficher l'image
		variables_["ImageViewed"] = &std::any_cast<ImageWidget*>(variables_["imageWidget"])->getImage();
		ImageViewerWindow();
        return true;
    });
    rightClickOnImage.setCallBack(1, [this](ClickEvent, ButtonWidget*)
    { // Modifier les tags
        updateFunction_ = [this]()
        {// Ouvrir la fenêtre tagger
            tagSetterWindow();
        };
        return true;
    });
    variables_["rightClickOnImage"] = rightClickOnImage;
    
    ListWidget rightClickOnTag({u8" Ajouter Tag ", u8" Supprimer Tag "}, true);

    // Contruction fenêtre
    std::unique_ptr<ListWidget> list(new ListWidget({u8" Fichier "}));
    list->setCallBack(0, [this](ClickEvent ce, ButtonWidget* but)
    {
        ListWidget* listFichier = new ListWidget(std::any_cast<ListWidget>(variables_["Fichier"]));
        but->getWindow()->spawnRightClickMenu(listFichier, dim_t{0, but->size().y});

        return true;
    });

    auto grid = new GridWidget(*collection_, window_.size().x, window_.size().y, {150, 150}, [this](ClickEvent ce, ImageWidget* iw)
        {
            if(ce.type == ClickEvent::RIGHT)
            {
                iw->getWindow()->spawnRightClickMenu(new ListWidget(std::any_cast<ListWidget>(variables_["rightClickOnImage"])));
                variables_["imageWidget"] = iw;
                return true;
            }
            else if(ce.type == ClickEvent::LEFT)
            {
                std::any_cast<TagSetterWidget*>(variables_["tagger"])->callRedraw();
            }
            return false;
        });
    std::unique_ptr<ScrollWidget> scroll(new ScrollWidget(grid, window_.size()));
    auto tagsetter = new TagSetterWidget(possibleTags_, *selected_, window_.size().x, window_.size().y);
    auto scroll2 = new ScrollWidget(tagsetter, window_.size());
    auto layout = new LayoutWidget(scroll.release(), scroll2, 2.0/3.0, window_.size());
    auto menubar = new MenuBarWidget(layout, list.release(), window_.size());
    window_.setContent(menubar);
    
    variables_["tagger"] = tagsetter;
}

void Application::ImageViewerWindow()
{
	// Construction des différents menus
	ListWidget fichierList = std::move(FichierList());
	variables_["Fichier"] = fichierList;
	selected_ = FilteredCollection<Image>(*collection_, [](const Image& image) { return image.isSelected(); });

	// Contruction fenêtre
	std::unique_ptr<ListWidget> list(new ListWidget({ u8" Fichier " }));
	list->setCallBack(0, [this](ClickEvent ce, ButtonWidget* but)
	{
		ListWidget* listFichier = new ListWidget(std::any_cast<ListWidget>(variables_["Fichier"]));
		but->getWindow()->spawnRightClickMenu(listFichier, dim_t{ 0, but->size().y });

		return true;
	});

	auto imagePtr = std::any_cast<Image*>(variables_["ImageViewed"]);

	auto image = new ImageWidget(*imagePtr,window_.size());
	auto tagViewer = new TagViewerWidget(imagePtr, window_.size().x, window_.size().y);
	auto scroller = new ScrollWidget(tagViewer, window_.size());
	auto layout = new LayoutWidget(image, scroller, 0.7, window_.size());
	auto menubar = new MenuBarWidget(layout, list.release(), window_.size());
	window_.setContent(menubar);
}
