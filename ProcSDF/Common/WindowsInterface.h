#pragma once
#include <Windows.h>
#include <string>
#include <shobjidl.h>
#include <commdlg.h>

class WindowsInterface {
public:
    static std::pair<bool, std::string> openFile();
    static std::pair<bool, std::string> saveFile(const char* p_filter);
};