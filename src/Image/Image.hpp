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

		//setters
		void setPath(std::experimental::filesystem::path p) { path_ = p; }
		void setTagList(TagList t) { taglist_ = t; }
		void setImgPtr(cimg_library::CImg<unsigned char>* cimgPtr) { cimg_ = std::unique_ptr<cimg_library::CImg<unsigned char>>(cimgPtr); }

		//méthodes de sauvegarde
		void save(std::ostream&) const;
		void load(std::istream&);

		//opérateur de lecture de flux
		friend std::istream& operator>>(std::istream& inStream, Image& image);

	private:
		std::unique_ptr<cimg_library::CImg<unsigned char>> cimg_ = nullptr; //Représentation d'image avec CImg
		std::experimental::filesystem::path path_; //Chemin de l'image
		TagList taglist_; //Liste des tags associés à l'image
};

#endif // !IMAGE_HPP
