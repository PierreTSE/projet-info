#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "TagList.hpp"
#include <experimental/filesystem>
#include <memory>

template<typename img_t>
class Image
{
	public:
		explicit Image(const std::experimental::filesystem::path& p, std::unique_ptr<img_t> imgPtr = nullptr, const TagList& t = {}) 
			: img_{ std::move(imgPtr) }, path_{ p }, taglist_{ t } {} //constructeur complet

		//getters
		std::experimental::filesystem::path getPath() { return path_; }
		TagList getTagList() { return taglist_; }
		img_t* getImgPtr() { return img_.get(); }

		//setters
		void setPath(std::experimental::filesystem::path p) { path_ = p; }
		void setTagList(TagList t) { taglist_ = t; }
		void setImgPtr(img_t* imgPtr) { img_ = imgPtr; }

	private:
		std::unique_ptr<img_t> img_ = nullptr; //TODO l'implémentation va varier (Cimg,SFML...)
		std::experimental::filesystem::path path_; //Chemin de l'image
		TagList taglist_; //Liste des tags associés à l'image
};

#endif // !IMAGE_HPP
