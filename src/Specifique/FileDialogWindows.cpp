﻿#include "FileDialog.hpp"
#include "../system_target.hpp"

#ifdef WINDOWS

#include <windows.h>
#include <shlobj.h>
#include <string>

using namespace std;
namespace fs = std::experimental::filesystem;

fs::path getOpenFileName(const string &title, const file_filter &filterIN, const fs::path &initialDir)
{
	std::string filter = filterIN.pattern;

	// Processing of the filter
	size_t const begin_pos = filter.find_first_of('(');
	size_t const end_pos = filter.find_last_of(')');
	size_t const nb = end_pos - begin_pos;
	string ext;
	if (begin_pos != string::npos && end_pos != string::npos && nb > 1) {
		ext = filter.substr(begin_pos + 1, nb - 1);
		for (unsigned i = 0; i < ext.size(); i++) if (ext[i] == ' ') ext[i] = ';';
	}
	string ext_filter = filter + '\0' + ext + '\0';

	char name[MAX_PATH] = "";
	OPENFILENAME of;
	memset(&of, 0, sizeof(of));
	of.lStructSize = sizeof(of);
	of.lpstrFile = name;
	of.lpstrInitialDir = initialDir.u8string().c_str();
	of.lpstrFilter = &ext_filter[0];
	of.nMaxFile = MAX_PATH;
	of.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	of.lpstrTitle = title.c_str();

	string name_string;
	if (GetOpenFileName(&of)) name_string = string(name);
	return name_string;
}

fs::path getSaveFileName(const string &title, const file_filter & filterIN, const std::experimental::filesystem::path & initialDir)
{
	std::string filter = filterIN.pattern;

	// Processing of the filter
	size_t const begin_pos = filter.find_first_of('(');
	size_t const end_pos = filter.find_last_of(')');
	size_t const nb = end_pos - begin_pos;
	string ext;
	if (begin_pos != string::npos && end_pos != string::npos && nb > 1) {
		ext = filter.substr(begin_pos + 1, nb - 1);
		for (unsigned i = 0; i < ext.size(); i++) if (ext[i] == ' ') ext[i] = ';';
	}
	string ext_filter = filter + '\0' + ext + '\0';

	char name[MAX_PATH] = "";
	OPENFILENAME of;
	memset(&of, 0, sizeof(of));
	of.lStructSize = sizeof(of);
	of.lpstrFile = name;
	of.lpstrInitialDir = initialDir.u8string().c_str();
	of.lpstrFilter = &ext_filter[0];
	of.nMaxFile = MAX_PATH;
	of.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
	of.lpstrTitle = title.c_str();

	string name_string;
	if (GetSaveFileName(&of)) name_string = string(name);
	return name_string;
}


static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{

	if (uMsg == BFFM_INITIALIZED)
	{
		string tmp = reinterpret_cast<const char *>(lpData);
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}

fs::path browseFolder(const string &title, const std::experimental::filesystem::path& initialDir)
{
	TCHAR path[MAX_PATH];
	TCHAR path_param[MAX_PATH];

	if (initialDir == "")
		GetCurrentDirectory(MAX_PATH, path_param);
	else
		lstrcpy(path_param, initialDir.u8string().c_str());

	BROWSEINFO bi = { nullptr };
	bi.lpszTitle = title.c_str();
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = reinterpret_cast<LPARAM>(path_param);

    const LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != nullptr)
	{
		//get the name of the folder and put it in path
		SHGetPathFromIDList(pidl, path);

		//free memory used
		IMalloc * imalloc = nullptr;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}

		return string(path);
	}

	return "";
}
#endif // WINDOWS