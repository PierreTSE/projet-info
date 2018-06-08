#include "Application.hpp"
#include "Interface/MenuBarWidget.hpp"
#include "Interface/TagSetterWidget.hpp"
#include "Interface/TagViewerWidget.hpp"
#include "Interface/GridWidget.hpp"
#include "Interface/LayoutWidget.hpp"
#include "Utilities/Utilities.hpp"
#include "Specifique/FileDialog.hpp"
#include "Interface/ScrollWidget.hpp"
#include "Utilities/TextBox/TextBox.hpp"
#include <chrono>
#include <thread>
#include <experimental/filesystem>
#include <iostream>
#include "Interface/TagSelector.hpp"


namespace fs = std::experimental::filesystem;
using namespace std::chrono_literals;

Application::Application() :
    window_{nullptr, {1000, 500}}
{
	wd_ = fs::current_path();
	window_.setCallBack([this](bool b)
	{
	    if(collection_)
	    {
			if (isSearching_)
				for (auto& image : searchedColl_.back())
					image.select(b);
			else
	            for(auto& image : *collection_)
				    image.select(b);
	    }
		return true;
	});

    if(!loadPossibleTags())
        std::clog << "No tag list saved." << std::endl;
    initialWindow();
}

Application::~Application()
{
	fs::current_path(wd_);
    if(collection_)
	    save();
	savePossibleTags();
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
	ListWidget fichierList({ u8" Nouveau ", u8" Ouvrir... ", u8" Importer... ", u8" Enregistrer ", u8" Enregistrer sous... ", u8" Fermer ", u8" Quitter " }, true);
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
	fichierList.setCallBack(6, [this](ClickEvent ce, ButtonWidget* but)
	{ // Quitter
		updateFunction_ = [this]()
		{
			window_.close();
		};
		return true;
	});

	return fichierList;
}

void Application::initialWindow()
{
	isSearching_ = false;
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
	isSearching_ = false;
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
    
    ListWidget rightClickOnImage({u8" Afficher Image ", u8" Modifier les tags des images s\u00E9lectionn\u00E9es ", u8" Enlever les images s\u00E9lectionn\u00E9es ", u8" Rechercher par tag "}, true);
    rightClickOnImage.setCallBack(0, [this](ClickEvent, ButtonWidget*)
        { // Afficher l'image
			variables_["ImageViewed"] = &std::any_cast<ImageWidget*>(variables_["imageWidget"])->getImage();
			imageViewerWindow();            
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
    rightClickOnImage.setCallBack(2, [this](ClickEvent, ButtonWidget*)
    { // Sppr les images
        updateFunction_ = [this]()
        {// Suppr les images
            for(size_t i = 0; i < collection_->size(); i++)
            {
                if(collection_->at(i).isSelected())
                {
                    auto it = collection_->begin();
                    std::advance(it, i);
                    collection_->erase(it);
                    i--;
                }
            }
            collectionWindow();
        };
        return true;
    });
    rightClickOnImage.setCallBack(3, [this](ClickEvent, ButtonWidget*)
    { // Recherche
        updateFunction_ = [this]()
        {// Suppr les images
            for(auto& image : *collection_)
                image.select(false); // Déselectionne toutes les images avant recherche
            imageSearchWindow();
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
	isSearching_ = false;
    // Construction des différents menus
    ListWidget fichierList = std::move(FichierList());
    variables_["Fichier"] = fichierList;
    selected_ = FilteredCollection<Image>(*collection_, [](const Image& image) { return image.isSelected(); });

    ListWidget rightClickOnImage({u8" Afficher Image ", u8" Retour \u00E0 la collection ", u8" Enlever les images s\u00E9lectionn\u00E9es "}, true);
    rightClickOnImage.setCallBack(0, [this](ClickEvent, ButtonWidget*)
    { // Afficher l'image
		variables_["ImageViewed"] = &std::any_cast<ImageWidget*>(variables_["imageWidget"])->getImage();
		imageViewerWindow();
        return true;
    });
    rightClickOnImage.setCallBack(1, [this](ClickEvent, ButtonWidget*)
    { // Retour
        updateFunction_ = [this]()
        {// Ouvrir la fenêtre tagger
            collectionWindow();
        };
        return true;
    });
    rightClickOnImage.setCallBack(2, [this](ClickEvent, ButtonWidget*)
    {
        updateFunction_ = [this]()
        {// Suppr les images
            for(size_t i = 0; i < collection_->size(); i++)
            {
                if(collection_->at(i).isSelected())
                {
                    auto it = collection_->begin();
                    std::advance(it, i);
                    collection_->erase(it);
                    i--;
                }
            }
            collectionWindow();
        };
        return true;
    });
    variables_["rightClickOnImage"] = rightClickOnImage;
    
    ListWidget rightClickOnTag({u8" Ajouter Tag ", u8" Supprimer Tag "}, true);
    rightClickOnTag.setCallBack(0, [this](ClickEvent, ButtonWidget*)
        { // Ajouter tag
            Tag newtag = getTextFromBox("Nouveau Tag");
            if(!newtag.empty())
                possibleTags_.insert(newtag);
			std::any_cast<TagSetterWidget*>(variables_["TagSetterPtr"])->callRedraw();
            return true;
        });
    rightClickOnTag.setCallBack(1, [this](ClickEvent, ButtonWidget*)
        { // Supprimer tag
            Tag tag = std::any_cast<Tag>(variables_["TagToDelete"]);
            possibleTags_.erase(tag);
            for(auto& image : *collection_)
                image.getTagList().erase(tag);
			std::any_cast<TagSetterWidget*>(variables_["TagSetterPtr"])->callRedraw();
            return true;
        });
    variables_["rightClickOnTagSetter"] = rightClickOnTag;

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
    auto tagsetter = new TagSetterWidget(possibleTags_, *selected_, window_.size().x, window_.size().y, [this](ClickEvent ce, TagSetterWidget* tsw, Tag tag)
        {
            if(ce.type == ClickEvent::RIGHT)
            {
                variables_["TagToDelete"] = tag;
                variables_["TagSetterPtr"] = tsw;
                tsw->getWindow()->spawnRightClickMenu(new ListWidget(std::any_cast<ListWidget>(variables_["rightClickOnTagSetter"])));
            }
            return true;
        });
    auto scroll2 = new ScrollWidget(tagsetter, window_.size());
    auto layout = new LayoutWidget(scroll.release(), scroll2, 2.0/3.0, window_.size());
    auto menubar = new MenuBarWidget(layout, list.release(), window_.size());
    window_.setContent(menubar);
    
    variables_["tagger"] = tagsetter;
}

void Application::imageViewerWindow()
{
	isSearching_ = false;
	// Construction des différents menus
	ListWidget fichierList = std::move(FichierList());
	variables_["Fichier"] = fichierList;
	selected_ = FilteredCollection<Image>(*collection_, [](const Image& image) { return image.isSelected(); });

    ListWidget rightClickOnImageViewer({u8" Retour \u00E0 la collection "}, true);
    rightClickOnImageViewer.setCallBack(0, [this](ClickEvent, ButtonWidget*)
    { // retour
        updateFunction_ = [this]()
        {// Ouvrir la fenêtre collection
            collectionWindow();
        };
        return true;
    });
    variables_["rightClickOnImageViewer"] = rightClickOnImageViewer;

	// Contruction fenêtre
	std::unique_ptr<ListWidget> list(new ListWidget({ u8" Fichier " }));
	list->setCallBack(0, [this](ClickEvent ce, ButtonWidget* but)
	{
		ListWidget* listFichier = new ListWidget(std::any_cast<ListWidget>(variables_["Fichier"]));
		but->getWindow()->spawnRightClickMenu(listFichier, dim_t{ 0, but->size().y });

		return true;
	});

	auto imagePtr = std::any_cast<Image*>(variables_["ImageViewed"]);

	auto image = new ImageWidget(*imagePtr,window_.size(), [this](ClickEvent ce, ImageWidget* iw)
    {
        if(ce.type == ClickEvent::RIGHT)
            iw->getWindow()->spawnRightClickMenu(new ListWidget(std::any_cast<ListWidget>(variables_["rightClickOnImageViewer"])));
        return true;
    });
	image->setSelectable(false);
	auto tagViewer = new TagViewerWidget(imagePtr, window_.size().x, window_.size().y);
	auto scroller = new ScrollWidget(tagViewer, window_.size());
	auto layout = new LayoutWidget(image, scroller, 0.7, window_.size());
	auto menubar = new MenuBarWidget(layout, list.release(), window_.size());
	window_.setContent(menubar);
}

void Application::imageSearchWindow()
{
	isSearching_ = true;

	// Construction des différents menus
	ListWidget fichierList = std::move(FichierList());
	variables_["Fichier"] = fichierList;
	
	ListWidget rightClickImageSearch({u8" Retour \u00E0 la collection "}, true);
	rightClickImageSearch.setCallBack(0, [this](ClickEvent, ButtonWidget*) {
	    updateFunction_ = [this]()
        {
            collectionWindow();
        };
	    return true;
	});
	variables_["rightClickImageSearch"] = rightClickImageSearch;

	// Contruction fenêtre
	std::unique_ptr<ListWidget> list(new ListWidget({ u8" Fichier " }));
	list->setCallBack(0, [this](ClickEvent ce, ButtonWidget* but)
	{
		ListWidget* listFichier = new ListWidget(std::any_cast<ListWidget>(variables_["Fichier"]));
		but->getWindow()->spawnRightClickMenu(listFichier, dim_t{ 0, but->size().y });
		return true;
	});

	searchedColl_.clear();
	searchedColl_.emplace_back(*collection_, [](const Image&){ return true; });
	for(auto& tag : filter_)
    {
        searchedColl_.emplace_back(searchedColl_.back(), [&tag](const Image& image) { return image.hasTag(tag); });
    }
	
	auto grid = new GridWidget(searchedColl_.back(), window_.size().x, window_.size().y, { 150, 150 }, [this](ClickEvent ce, ImageWidget* iw) {
	    iw->getWindow()->spawnRightClickMenu(new ListWidget(std::any_cast<ListWidget>(variables_["rightClickImageSearch"])));
	    return true; 
	});
	auto scroll = new ScrollWidget(grid, window_.size());
	auto tagSelector = new TagSelector(possibleTags_, filter_, window_.size().x,window_.size().y);
	tagSelector->setCallBack([this](ClickEvent, TagSelector*, TagList tl) {
	    filter_ = tl;
	    updateFunction_ = [this](){
	        imageSearchWindow();
	    };
	    return true;
	});
	auto scroller = new ScrollWidget(tagSelector, window_.size());
	auto layout = new LayoutWidget(scroller, scroll, 0.3, window_.size());
	auto menubar = new MenuBarWidget(layout, list.release(), window_.size());
	window_.setContent(menubar);
}
