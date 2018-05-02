#include <experimental/filesystem>
#include "FileDialog.hpp"
#include "FilteredCollection.hpp"
#include <iostream>

namespace fs = std::experimental::filesystem;

bool isPair(const int& i)
{
	return (i % 2 == 0);
}

int main()
{
	std::vector<int> vect = { 3,0,12,15,1,2 };

	FilteredCollection filteredCollec(vect, isPair);
	std::cout << filteredCollec.front() << std::endl;

	filteredCollec.front() = 8;

	for (auto e : vect) std::cout << e << " ";

    //std::cout << fs::current_path().string() << std::endl;
    return 0;
}