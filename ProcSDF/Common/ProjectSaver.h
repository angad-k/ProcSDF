#pragma once
#include<json/json.h>
#include "GUI/Nodes/Node.h"
#include "Rendering/Materials/Material.h"
class ProjectSaver {
private:
	static Node* getNodeFromNodeName(std::string p_nodeName, int p_ID);
	static void saveWorldSettings(Json::Value& p_value);
	static void saveRenderSettings(Json::Value& p_value);
	static void saveNodeGraphSettings(Json::Value& p_value);
	static void saveNodeIDs(Json::Value& p_value);
	static void saveNodeLinks(Json::Value& p_value);
	static void saveNodeList(Json::Value& p_value);
	static void saveNode(Json::Value& p_value, Node* p_node, bool p_isCustom, std::string p_filePath, int p_materialID);
	static void saveMaterialSettings(Json::Value& p_value);
	static void saveMaterialList(Json::Value& p_value);
	static void saveMaterial(Json::Value& p_value, Material* p_material);
	static void saveMaterialIDs(Json::Value& p_value);
	static std::string getFileNameFromFilePath(std::string p_filePath);
	static bool saveProjectToFile(Json::Value& p_value);

	static bool fetchFileAndContent(std::string &p_fileContent);
	static void clearNodeGraph();
	static bool parseFileContent(std::string p_fileContent, Json::Value& p_value);
	static bool parseWorldSettings(const Json::Value& p_value);
	template<typename T>
	static bool parseColor(const Json::Value& p_value, T &p_color);
	static bool parseRenderingSettings(const Json::Value& p_value);
public:
	static bool saveProject();
	static bool loadProject();
};
