#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "TagList.hpp"
#include "../CImg.h"
#include <experimental/filesystem>
#include <fstream>
#include <memory>


class Image
{
    public:
        explicit Image() = default;
        explicit Image(const std::experimental::filesystem::path& p, std::unique_ptr<cimg_library::CImg<unsigned char>> cimgPtr = nullptr, const TagList& t = {}) 
            : cimg_{ std::move(cimgPtr) }, path_{ p }, taglist_{ t } {} //constructeur complet
    
        //getters
        std::experimental::filesystem::path& getPath() { return path_; }
        const std::experimental::filesystem::path& getPath() const { return path_; }
        TagList& getTagList() { return taglist_; }
        const TagList& getTagList() const { return taglist_; }
        cimg_library::CImg<unsigned char>* getImgPtr() { return cimg_.get(); }
        const cimg_library::CImg<unsigned char>* getImgPtr() const { return cimg_.get(); }
    
        //setters
        void setPath(std::experimental::filesystem::path p) { path_ = std::move(p); }
        void setTagList(TagList t) { taglist_ = std::move(t); }
        void setImgPtr(cimg_library::CImg<unsigned char>* cimgPtr) { cimg_ = std::unique_ptr<cimg_library::CImg<unsigned char>>(cimgPtr); }
    
        //chargement d'une CImg
        void loadImage();
    
        //opérateur de lecture de flux
        friend std::istream& operator>>(std::istream& inStream, Image& image);
        friend std::ostream& operator<<(std::ostream& outStream, const Image& image);
        
        bool isSelected() const { return selected_; }
        void select(bool b) { selected_ = b; }
    
        bool hasTag(const Tag& tag) const { return taglist_.find(tag) != taglist_.end(); }
    
    private:
        std::unique_ptr<cimg_library::CImg<unsigned char>> cimg_ = nullptr; //Représentation d'image avec CImg
        std::experimental::filesystem::path path_; //Chemin de l'image
        TagList taglist_; //Liste des tags associés à l'image
        bool selected_ = false;
};

#endif // !IMAGE_HPP
