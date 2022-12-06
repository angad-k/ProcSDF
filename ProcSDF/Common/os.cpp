#include "os.h"

std::string OS::fetchFileContent(std::string p_filePath) {
	std::string l_fileContent = "\n";
	std::ifstream sourceFile(p_filePath);
	l_fileContent.append((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
	l_fileContent.append("\n");
	return l_fileContent;
}

std::pair<bool, std::string> OS::pickFile() {
	std::pair<bool, std::string> l_filePath = WindowsInterface::openFile();
	if (l_filePath.first)
	{
		PRINT("File picked : " + l_filePath.second);
	}
	else
	{
		PRINT("File not picked.");
	}
	return l_filePath;
}

// if file exists overwrite it else create it and save the file.
bool OS::saveFileContent(std::string p_filePath, std::string p_fileContent) {
	std::ofstream l_fileStream(p_filePath, std::ofstream::trunc);

	if (!l_fileStream) {
		ERR("File not saved");
		return false;
	}

	l_fileStream << p_fileContent;
	l_fileStream.close();
}