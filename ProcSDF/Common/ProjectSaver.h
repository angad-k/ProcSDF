#pragma once
#include<json/json.h>
#include "GUI/Nodes/Node.h"
class ProjectSaver {
private:
	static Node* getNodeFromNodeName(std::string p_nodeName, int p_ID);
	static void saveWorldSettings(Json::Value& p_value);
	static void saveRenderSettings(Json::Value& p_value);
	static void saveNodeGraphSettings(Json::Value& p_value);
	static void saveNodeIDs(Json::Value& p_value);
	static void saveNodeLinks(Json::Value& p_value);
	static void saveNodeList(Json::Value& p_value);
	static void saveNode(Json::Value& p_value, Node* p_node, bool p_isCustom, std::string p_filePath);
	static std::string getFileNameFromFilePath(std::string p_filePath);
public:
	static bool saveProject();
	static bool loadProject();
};
