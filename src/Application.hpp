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


/**
 * \class Application
 * \brief Gère l'application
 * 
 * Cette classe regroupe toutes les données et les actions principales de l'application
 * Elle va s'occuper de charger les collections d'images et de les afficher dans une fenêtre
 */
class Application
{
    public:
        /**
         * @brief Initialise les données du programme
         */
        Application();
        /**
         * @brief Effectue une sauvegarde de la collection et enregistre les tags
         */
        ~Application();
        /**
         * @brief Lance l'application
         * @return un code d'état qui correspond au code d'état attendu en retour de la fonction main
         */
        int execute();

        /**
         * @brief Lance la fenêtre modale qui demande un fichier d'enregistre ment puis appelle la fonction @c actualSave
         * @return un booléen vrai tout si s'est bien passé et faux 
         */
        bool saveAs();
        /**
         * @brief Lance la fonction saveAs s'il n'existe pas de sauvegarde antérieure et actualSave s'il en existe une
         * @return un booléen vrai tout si s'est bien passé et faux  
         */
        bool save();

        /**
         * @brief Charge la liste des tags accessibles à l'utilisateur
         * @return un booléen vrai tout si s'est bien passé et faux  
         */
		bool loadPossibleTags();
		/**
		 * @brief Enregistre la liste des tags accéssibles à l'utilisateur
		 */
		void savePossibleTags() const;

    private:
        /**
         * @brief Appelle la fonction de mise à jour si elle existe, cette dernière peut être attribué de façon dynamique
         * par un composant de la fenêtre pour pouvoir changer d'état
         */
        void update();
        
        /**
         * @brief Crée et retourne le menu "Fichier"
         * @return Le menu "Fichier"
         */
        ListWidget FichierList();
    
        /**
         * @brief Construit la fenêtre initiale
         */
        void initialWindow();
        /**
         * @brief Construit la fenêtre de visualisation de collection
         */
        void collectionWindow();
        /**
         * @brief Construit la fenêtre d'attribution de tags
         */
        void tagSetterWindow();
        /**
         * @brief Construit la fenêtre de visualisation d'image
         */
		void imageViewerWindow();
		/**
		 * @brief Construit la fenêtre de recherche
		 */
		void imageSearchWindow();

		/**
		 * @brief Sauvegarde la collection actuelle
		 */
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
