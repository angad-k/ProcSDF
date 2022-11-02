#pragma once
#include <Windows.h>
#include <string>
#include <shobjidl.h>

namespace WindowsInterface {
    std::pair<bool, std::string> openFile();
}