#pragma once
#include <Windows.h>
#include <string>
#include <shobjidl.h>

class WindowsInterface {
public:
    static std::pair<bool, std::string> openFile();
};