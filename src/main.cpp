#include "Collection/CollectionIterator.hpp"
#include "Collection/FilteredCollection.hpp"
#include "Collection/Collection.hpp"
#include "Collection/CollectionPool.hpp"
#include "FileDialog/FileDialog.hpp"
#include "Image/Image.hpp"
#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <exception>

namespace fs = std::experimental::filesystem;

bool isPair(const int& i)
{
	return (i % 2 == 0);
}

template <typename img_t>
CollectionPool<Image<img_t>> getPoolFromDirectory()
{
	CollectionPool<Image<img_t>> collectionPool;

	fs::path directoryPath = browseFolder();
	if (fs::is_directory(directoryPath))
	{
		TagList taglist;

		for (auto& file : fs::directory_iterator(directoryPath))
		{
			if (file.path().extension() == "txt")
			{
				std::ifstream tagListFile(file.path(), std::ios::in);
				//TODO gestion de la tagList particulière
				break;
			}
		}
		for (auto& file : fs::directory_iterator(directoryPath))
		{
			if (file.path().extension() == "ppm")
			{
				Image<img_t> img(file.path(), nullptr, {})); //TODO gérer le fichier image, ajouter tagList
				collectionPool.push_back(img);
			}
		}
	}
	else
	{
		throw std::runtime_error;
	}
}


int main()
{







	//fs::path path;
	//path = getSaveFileName();
	//Image<int> image(path);

	//std::cout << path << std::endl;



	//CollectionPool<int> collectionPool = {1,5,1,17,1,1,4,94,89};
	//for (auto e : collectionPool)
	//	std::cout << e << " ";

	//FilteredCollection<int> filteredcollection(collectionPool,isPair);

	/*int i = 0;
	for (auto e : filteredcollection)
	{
		++i;
		if(i > 1000)
			std::terminate();
		std::cout << e << " ";
	}
	std::cout << std::endl;
	
	FilteredCollection<int> fc(filteredcollection, [](const int& a){ return a%3==0;});
	i = 0;
	for (auto e : fc)
	{
		++i;
		if(i > 1000)
			std::terminate();
		std::cout << e << " ";
	}*/
	//using iterator = CollectionIterator<int>;

	//using type = std::iterator_traits<iterator>::value_type;
	//type a = 3;


	//std::vector<int> vect = { 3,0,12,15,1,2 };

	//FilteredCollection filteredCollec(vect, isPair);
	//std::cout << filteredCollec.front() << std::endl;

	//filteredCollec.front() = 8;

	//for (auto e : vect) std::cout << e << " ";

    //std::cout << fs::current_path().string() << std::endl;
    return 0;
}