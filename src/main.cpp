#include "CollectionIterator.hpp"
#include "FileDialog.hpp"
#include "FilteredCollection.hpp"
#include "Collection.hpp"
#include "CollectionPool.hpp"
#include "Image.hpp"
#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

bool isPair(const int& i)
{
	return (i % 2 == 0);
}


int main()
{
	fs::path path;
	path = getSaveFileName();
	Image<int> image(path);





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