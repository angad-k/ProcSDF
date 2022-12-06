#pragma once
#include<json/json.h>
class ProjectSaver {
private:
	static bool syntaxChecker(std::string p_jsonString);
public:
	static bool saveProject();
	static bool loadProject();
};
