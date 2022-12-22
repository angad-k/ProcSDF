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
	static bool saveFileContent(std::string p_filePath, std::string p_fileContent);
	static std::pair<bool, std::string> pickDestination(const char* p_filter);
	static std::pair<bool, std::string> pickSaveAsFile();
	static std::pair<bool, std::string> pickRenderToFile();
};