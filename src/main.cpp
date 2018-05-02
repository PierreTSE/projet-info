#include "CollectionIterator.hpp"
#include "FileDialog.hpp"
#include "FilteredCollection.hpp"
#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

bool isPair(const int& i)
{
	return (i % 2 == 0);
}




int main()
{
	int a = 5;
	CollectionIterator<int> it1(nullptr);
	CollectionIterator<int> it2(reinterpret_cast<IteratorBase<int>*>(&a));
	std::cout << it1.itr_ << " " << it2.itr_ << std::endl;
	std::swap(it1, it2);
	std::cout << it1.itr_ << " " << it2.itr_ << std::endl;









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