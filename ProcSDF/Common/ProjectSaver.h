#pragma once
#include<json/json.h>
#include "GUI/Nodes/Node.h"
class ProjectSaver {
private:
	static Node* getNodeFromNodeName(std::string p_nodeName, int p_ID);
	static void saveWorldSettings(Json::Value& value);
	static void saveRenderSettings(Json::Value& value);
public:
	static bool saveProject();
	static bool loadProject();
};
