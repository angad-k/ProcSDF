#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "Common/WindowsInterface.h"
#include "Common/logger.h"
class OS {
private:
	static std::string fixExtension(std::string p_str, std::string p_suffix)
	{
		if (p_str.length() < p_suffix.length())
		{
			p_str += p_suffix;
		}
		else if (p_str.compare(p_str.length() - p_suffix.length(), p_suffix.length(), p_suffix) != 0)
			p_str += p_suffix;
		return p_str;
	}
public:
	static std::string fetchFileContent(std::string p_filePath);
	static std::pair<bool, std::string> pickFile();
	static bool saveFileContent(std::string p_filePath, std::string p_fileContent);
	static std::pair<bool, std::string> pickDestination(const char* p_filter);
	static std::pair<bool, std::string> pickSaveAsFile();
	static std::pair<bool, std::string> pickRenderToFile();
};