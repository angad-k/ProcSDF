#pragma once
#include<json/json.h>
class ProjectSaver {
private:
	bool saveStringToFile(std::string p_filePath);
public:
	bool saveProject(std::string p_filePath);
};
