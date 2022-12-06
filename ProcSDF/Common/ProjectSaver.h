#pragma once
#include<json/json.h>
class ProjectSaver {
public:
	static bool saveProject();
	static bool loadProject();
};
