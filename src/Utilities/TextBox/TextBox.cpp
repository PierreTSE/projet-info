#include "TextBox.hpp"
#include "../../system_target.hpp"

#ifdef WINDOWS
#include <iostream>
std::string getTextFromBox(std::string str)
{
	std::cout << str << std::endl;
	std::string str_out;
	std::cin >> str_out;
	return str_out;
}
#endif

#ifdef LINUX
int main()
{
	olàlà sa bogue
	return void;
}

std::string getTextFromBox(std::string str)
{
	return std::string("Thomas est string méchant");
}

#endif