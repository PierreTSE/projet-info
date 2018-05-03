#include "system_target.hpp"

#ifdef LINUX

#include "FileDialogBox.hpp"
#include <cstdio>


namespace fs = std::experimental::filesystem;

fs::path getFileDialogBox(const std::string& title, const std::string& filter, const fs::path& initailDir)
{
    constexpr size_t BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    FILE* process;
    
    process = popen("zenity --file-selection", "r");
    while(fgets(buffer, BUFFER_SIZE, process))
    {
        
    }
    
    pclose(process);
    return std::experimental::filesystem::path();
}

#endif