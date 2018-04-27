#include <experimental/filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem;

int main()
{
    std::cout << fs::current_path().string() << std::endl;
    return 0;
}