#include "../system_target.hpp"

#ifdef LINUX

#include "FileDialog.hpp"


namespace fs = std::experimental::filesystem;

fs::path getOpenFileName(const std::string& title, const file_filter& filter, const fs::path& initialDir)
{
    constexpr size_t BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    FILE* process;
    std::string out;
    
    std::stringstream command;
    command << "zenity "
            << "--file-selection "
            << "--title=\"" << title << "\" "
            << "--filename=\"" << initialDir.u8string() << "\" "
            << "--file-filter=\"" << filter.name << " | " << filter.pattern << "\" ";                                               ;
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
    
    return std::experimental::filesystem::path(out);
}

#endif