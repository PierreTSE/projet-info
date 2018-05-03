#include "CollectionIterator.hpp"
#include "FileDialog.hpp"
#include "FilteredCollection.hpp"
#include "Collection.hpp"
#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

bool isPair(const int& i)
{
	return (i % 2 == 0);
}

//c'est bon les accents Thomas ? --> éàèùç


int main()
{
	Collection<int> c;


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