#include "TextBox.hpp"
#include "../../system_target.hpp"


#ifdef WINDOWS

#include <iostream>
#include "Windows.h"
#include <cstdio>


std::string getTextFromBox(std::string str)
{
	std::cout << str << std::endl;
	std::string str_out;
	std::cin >> str_out;
	return str_out;
}

std::string pOpen(std::string)
{
	constexpr size_t BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	std::string out;

	FILE* process = _popen("test.exe", "r");
	while (fgets(buffer, BUFFER_SIZE, process))
	{
		out += buffer;
	}
	_pclose(process);

	out.pop_back();
	return out;
}
#endif

#ifdef LINUX

#include <sstream>


std::string getTextFromBox(std::string str)
{
	constexpr size_t BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	FILE* process;
	std::string out;

	std::stringstream command;
	command << "zenity "
			<< "--entry "
			<< "--title=\"" << str << "\" ";                                            ;
	process = popen(command.str().c_str(), "r");
	while(fgets(buffer, BUFFER_SIZE, process))
	{
		out += buffer;
	}
	pclose(process);

	auto pos = out.rfind('\n');
	if(pos != std::string::npos)
	{
		out = out.substr(0, pos);
		pos = out.rfind('\n');
		if(pos != std::string::npos)
			out = out.substr(pos);
	}

	return out;
}

#endif