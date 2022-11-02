#include <string>
#include <fstream>
#include <iostream>
namespace OS {
	static std::string fetchFileContent(std::string p_filePath) {
		std::string l_fileContent = "\n";
		std::ifstream sourceFile(p_filePath);
		l_fileContent.append((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
		l_fileContent.append("\n");
		return l_fileContent;
	}
}