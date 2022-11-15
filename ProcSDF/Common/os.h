#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "Common/WindowsInterface.h"
#include "Common/logger.h"
class OS {
public:
	static std::string fetchFileContent(std::string p_filePath);
	static std::pair<bool, std::string> pickFile();
};