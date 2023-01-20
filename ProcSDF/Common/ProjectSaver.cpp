#include "Common/ProjectSaver.h"
#include "GUI/NodeGraph.h"
#include "Common/constant.h"
#include "Common/WindowsInterface.h"
#include "Common/os.h"
#include "Rendering/Renderer.h"
#include "GUI/Nodes/FinalNode.h"
#include "GUI/Nodes/ObjectNode.h"
#include "GUI/Nodes/OperationNodes.h"
#include "GUI/Nodes/PrimitiveNodes.h"
#include "GUI/Nodes/TransformNodes.h"
#include "GUI/Nodes/CustomNode.h"
#include "Rendering/Materials/Material.h"
#include "Rendering/Materials/CustomMaterial.h"
#include "Rendering/Materials/Dielectric.h"
#include "Rendering/Materials/Diffuse.h"
#include "Rendering/Materials/Light.h"
#include "Rendering/Materials/Metal.h"

#define __PARSE_SUCESS__(status) if (!status) { return false;}
#define __IS_MEMBER_CHECK__(jsonValue, member) if (!jsonValue.isMember(member)) {return false;}

bool ProjectSaver::saveProject() {
	
	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
	Renderer* l_renderer = Renderer::getSingleton();
	Json::Value l_jsonValue;

	ProjectSaver::saveWorldSettings(l_jsonValue[save_project::world_settings::WORLD_SETTINGS]);

	ProjectSaver::saveRenderSettings(l_jsonValue[save_project::render_settings::RENDER_SETTINGS]);

	ProjectSaver::saveNodeGraphSettings(l_jsonValue[save_project::node_graph_settings::NODE_GRAPH_SETTINGS]);

	ProjectSaver::saveMaterialSettings(l_jsonValue[save_project::material_settings::MATERIAL_SETTINGS]);

	bool l_status = ProjectSaver::saveProjectToFile(l_jsonValue);

	return l_status;
}

bool ProjectSaver::saveProjectToFile(Json::Value& p_value) {
	
	Json::StyledWriter l_styledWriter;

	std::pair<bool, std::string> l_filePathInfo = OS::pickSaveAsFile();

	if (l_filePathInfo.first) {
		bool status = OS::saveFileContent(l_filePathInfo.second, std::string(l_styledWriter.write(p_value)));
		return status;
	}

	return false;
}

void ProjectSaver::saveWorldSettings(Json::Value& p_value) {

	Renderer* l_renderer = Renderer::getSingleton();
	for (int i = 0; i < 3; i++) {
		p_value[save_project::world_settings::CAMERA_ORIGIN][i] = l_renderer->getCameraOrigin()[i];
		p_value[save_project::world_settings::TOP_COLOR][i] = l_renderer->m_horizon_top_color[i];
		p_value[save_project::world_settings::BOTTOM_COLOR][i] = l_renderer->m_horizon_bottom_color[i];
	}
	p_value[save_project::world_settings::CAMERA_FOCAL_LENGTH] = *(l_renderer->getFocalLength());
}

void ProjectSaver::saveRenderSettings(Json::Value& p_value) {
	Renderer* l_renderer = Renderer::getSingleton();

	for (int i = 0; i < l_renderer->m_render_uniforms_values.size(); i++) {
		p_value[save_project::render_settings::RENDER_VALUES][i] = l_renderer->m_render_uniforms_values[i];
	}

	for (int i = 0; i < l_renderer->m_render_uniforms_debug_values.size(); i++) {
		p_value[save_project::render_settings::RENDER_DEBUG_VALUES][i] = (bool)l_renderer->m_render_uniforms_debug_values[i];
	}

	for (int i = 0; i < l_renderer->m_render_uniform_debug_cols.size(); i++) {
		for (int j = 0; j < 3; j++) {
			p_value[save_project::render_settings::RENDER_DEBUG_COLORS][i][j] = l_renderer->m_render_uniform_debug_cols[i][j];
		}
		
	}

}

void ProjectSaver::saveNodeGraphSettings(Json::Value& p_value) {
	
	ProjectSaver::saveNodeIDs(p_value[save_project::node_graph_settings::NODE_ID]);
	ProjectSaver::saveNodeLinks(p_value[save_project::node_graph_settings::NODE_LINK]);
	ProjectSaver::saveNodeList(p_value);

}

void ProjectSaver::saveNodeIDs(Json::Value& p_value) {

	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();

	int l_index = 0;
	for (Node* nd : l_nodeGraph->m_nodes) {
		p_value[l_index] = nd->m_ID;
		l_index++;
	}
}

void ProjectSaver::saveNodeLinks(Json::Value& p_value) {

	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();

	int l_index = 0;
	for (std::pair<int, int> link : l_nodeGraph->m_links) {
		p_value[l_index][0] = link.first;
		p_value[l_index][1] = link.second;
		l_index++;
	}
}

void ProjectSaver::saveNodeList(Json::Value& p_value) {

	// save minimal details for each node
	/*
	  "$(m_ID) : {
	   "nodeName" : $(m_nodeName),
	   "inputIDs" : $(m_inputIDs),
	   "outputIDS": $(m_outputIDs),
	   "inputFloat3" : $(m_inputFloat3),
	   "inputFloat" : $(m_inputFloat)
	   "filePath" : $(filePath) - if node is a custom Node
	  }
	*/
	
	std::map<int, int> l_nodeIDToMaterialID;

	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();

	for (Node* node : l_nodeGraph->m_nodes) {
		if (node->m_isObjectNode) {
			ObjectNode* l_objectNode = (ObjectNode*)node;
			l_nodeIDToMaterialID[l_objectNode->m_ID] = l_objectNode->getMaterialID();
		}
	}

	for (Node* node : l_nodeGraph->m_nodes) {
		
		bool l_isCustom = false;
		std::string l_filePath = l_nodeGraph->getCustomNodeFilePath(node->m_nodeName);

		if (l_filePath != "") {
			l_isCustom = true;
		}

		if (l_isCustom) {
			ProjectSaver::saveCustomNodeFileContent(p_value[save_project::node_graph_settings::CUSTOM_FILE_CONTENT], node);
		}
		ProjectSaver::saveNode(p_value[std::to_string(node->m_ID)], node, l_isCustom, l_filePath, l_nodeIDToMaterialID[node->m_ID]);
	}

}

void ProjectSaver::saveCustomNodeFileContent(Json::Value& p_value, Node* p_node) {

	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();

	std::string l_fileName = l_nodeGraph->getCustomNodeFilePath(p_node->m_nodeName);
	std::string l_fileContent = l_nodeGraph->getCustomNodeFileContentsfromNodeName(p_node->m_nodeName);

	p_value[ProjectSaver::getFileNameFromFilePath(l_fileName)] = l_fileContent;
}

void ProjectSaver::saveNode(Json::Value& p_value, Node* p_node, bool p_isCustom, std::string p_filePath, int p_materialID) {

	p_value[save_project::node_graph_settings::NODE_NAME] = p_node->m_nodeName;

	for (int i = 0; i < p_node->m_inputIDs.size(); i++) {
		p_value[save_project::node_graph_settings::INPUT_IDS][i] = p_node->m_inputIDs[i];
	}

	for (int i = 0; i < p_node->m_outputIDs.size(); i++) {
		p_value[save_project::node_graph_settings::OUTPUT_IDS][i] = p_node->m_outputIDs[i];
	}

	for (int i = 0; i < p_node->m_inputFloat3.size(); i++) {
		p_value[save_project::node_graph_settings::INPUT_FLOAT3][i][0] = p_node->m_inputFloat3[i][0];
		p_value[save_project::node_graph_settings::INPUT_FLOAT3][i][1] = p_node->m_inputFloat3[i][1];
		p_value[save_project::node_graph_settings::INPUT_FLOAT3][i][2] = p_node->m_inputFloat3[i][2];
	}

	for (int i = 0; i < p_node->m_inputFloats.size(); i++) {
		p_value[save_project::node_graph_settings::INPUT_FLOAT][i] = p_node->m_inputFloats[i];
	}

	if (p_isCustom) {
		p_value[save_project::node_graph_settings::FILE_NAME] = ProjectSaver::getFileNameFromFilePath(p_filePath);
	}

	if (p_node->m_isObjectNode) {
		p_value[save_project::node_graph_settings::MATERIAL_ID] = std::to_string(p_materialID);
	}
}

void ProjectSaver::saveMaterialSettings(Json::Value& p_value) {

	ProjectSaver::saveMaterialList(p_value);
	ProjectSaver::saveMaterialIDs(p_value[save_project::material_settings::MATERIAL_ID]);
}

void ProjectSaver::saveMaterialIDs(Json::Value& p_value) {

	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();

	int l_index = 0;
	for (Material* material : l_nodeGraph->getMaterials()) {
		p_value[l_index] = material->getID();
		l_index++;
	}
}
void ProjectSaver::saveMaterialList(Json::Value& p_value) {


	/*
	   Save minimal Material info
	   {
		 "color" : [r,g,b],
		 "inputFloats" : [],
		 "inputFloat3s" : [[],[]],
		 "materialType" : $(type)
	   }
	*/
	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
	for (Material* material : l_nodeGraph->getMaterials()) {
		ProjectSaver::saveMaterial(p_value[std::to_string(material->getID())], material);
		if (material->m_materialType == material_type::CUSTOM) {
			ProjectSaver::saveCustomMaterialFileContent(p_value[save_project::material_settings::CUSTOM_FILE_CONTENT], material);
		}
	}
}

void ProjectSaver::saveCustomMaterialFileContent(Json::Value& p_value, Material* p_material) {

	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();

	CustomMaterial* l_customMaterial = (CustomMaterial*)p_material;
	std::string l_fileName = ProjectSaver::getFileNameFromFilePath(
		l_nodeGraph->getCustomMaterialFilePathFromMaterialName(l_customMaterial->getCustomName()));
	std::string l_fileContent = l_nodeGraph->getCustomMaterialFileContentsfromMaterialName(l_customMaterial->getCustomName());

	p_value[l_fileName] = l_fileContent;
}

void ProjectSaver::saveMaterial(Json::Value& p_value, Material* p_material) {

	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();

	p_value[save_project::material_settings::MATERIAL_TYPE] = p_material->m_materialType;
	
	for (int i = 0; i < 3; i++) {
		p_value[save_project::material_settings::COLOR][i] = p_material->getColor()[i];
	}

	for (int i = 0; i < p_material->getInputFloats().size(); i++) {
		p_value[save_project::material_settings::INPUT_FLOAT][i] = p_material->getInputFloats()[i];
	}

	for (int i = 0; i < p_material->getInputFloat3s().size(); i++) {
		for (int j = 0; j < 3; j++) {
			p_value[save_project::material_settings::INPUT_FLOAT3][i][j] = p_material->getInputFloat3s()[i][j];
		}
	}

	if (p_material->m_materialType == material_type::CUSTOM) {
		CustomMaterial* l_customMaterial = (CustomMaterial*)p_material;
		p_value[save_project::material_settings::FILE_NAME] = ProjectSaver::getFileNameFromFilePath(
			l_nodeGraph->getCustomMaterialFilePathFromMaterialName(l_customMaterial->getCustomName()));
		p_value[save_project::material_settings::CUSTOM_NAME] = l_customMaterial->getCustomName();
	}


}

std::string ProjectSaver::getFileNameFromFilePath(std::string p_filePath) {

	int l_index = p_filePath.find_last_of('\\') + 1;
	return p_filePath.substr(l_index, p_filePath.length() - l_index);
}

Node* ProjectSaver::getNodeFromNodeName(std::string p_nodeName, int p_ID) {
	Node* l_node = NULL;
	if (p_nodeName == node_name::FINAL_NODE) {
		l_node = new FinalNode(p_ID);
	}
	else if (p_nodeName == node_name::OBJECT_NODE) {
		l_node = new ObjectNode(p_ID);
	}
	else if (p_nodeName == node_name::INTERSECTION_NODE) {
		l_node = new IntersectionNode(p_ID);
	}
	else if (p_nodeName == node_name::UNION_NODE) {
		l_node = new UnionNode(p_ID);
	}
	else if (p_nodeName == node_name::SPHERE_NODE) {
		l_node = new SphereNode(p_ID);
	}
	else if (p_nodeName == node_name::BOX_NODE) {
		l_node = new BoxNode(p_ID);
	}
	else if (p_nodeName == node_name::TORUS_NODE) {
		l_node = new TorusNode(p_ID);
	}
	else if (p_nodeName == node_name::BOX_FRAME_NODE) {
		l_node = new BoxFrameNode(p_ID);
	}
	else if (p_nodeName == node_name::TRANSLATION_NODE) {
		l_node = new TranslationNode(p_ID);
	}
	else if (p_nodeName == node_name::ROTATIONX_NODE) {
		l_node = new RotationNodeX(p_ID);
	}
	else if (p_nodeName == node_name::ROTATIONY_NODE) {
		l_node = new RotationNodeY(p_ID);
	}
	else if (p_nodeName == node_name::ROTATIONZ_NODE) {
		l_node = new RotationNodeZ(p_ID);
	}

	return l_node;
}

Material* ProjectSaver::getMaterialFromName(std::string p_materialType, int p_ID) {

	Material* l_material = NULL;

	if (p_materialType == material_type::DIFFUSE) {
		l_material = new Diffuse(p_ID);
	}
	else if (p_materialType == material_type::DIELECTRIC) {
		l_material = new Dielectric(p_ID);
	}
	else if (p_materialType == material_type::LIGHT) {
		l_material = new Light(p_ID);
	}
	else if (p_materialType == material_type::METAL) {
		l_material = new Metal(p_ID);
	}

	return l_material;
}

bool ProjectSaver::loadProject() {
	
	std::string l_jsonString;
	bool l_status = true;
	Json::Value l_value;
	l_status = ProjectSaver::fetchFileAndContent(l_jsonString);

	__PARSE_SUCESS__(l_status)

	ProjectSaver::clearNodeGraph();
	l_status = ProjectSaver::parseFileContent(l_jsonString, l_value);

	__PARSE_SUCESS__(l_status)

	l_status = ProjectSaver::parseWorldSettings(l_value);

	__PARSE_SUCESS__(l_status)

	l_status = ProjectSaver::parseRenderingSettings(l_value);

	__PARSE_SUCESS__(l_status)

	l_status = ProjectSaver::parseNodeGraphSettings(l_value);

	__PARSE_SUCESS__(l_status)

	l_status = ProjectSaver::parseMaterialSettings(l_value);

	__PARSE_SUCESS__(l_status)

	NodeGraph::getSingleton()->recompileNodeGraph();
	return true;
}

bool ProjectSaver::fetchFileAndContent(std::string& p_fileContent) {

	std::pair<bool, std::string> l_filePath = OS::pickFile();

	if (!l_filePath.first) {
		return false;
	}

	p_fileContent = OS::fetchFileContent(l_filePath.second);
	return true;
}

void ProjectSaver::clearNodeGraph() {
	NodeGraph::getSingleton()->clear();
}

bool ProjectSaver::parseFileContent(std::string p_fileContent, Json::Value& p_value) {

	Json::Reader l_reader;
	return l_reader.parse(p_fileContent, p_value);
}

bool ProjectSaver::parseNodeGraphSettings(const Json::Value& p_value) {

	__IS_MEMBER_CHECK__(p_value, save_project::node_graph_settings::NODE_GRAPH_SETTINGS)
	
	bool l_status = ProjectSaver::parseNodes(p_value[save_project::node_graph_settings::NODE_GRAPH_SETTINGS]);

	__PARSE_SUCESS__(l_status)

	l_status = ProjectSaver::parseNodeLink(p_value[save_project::node_graph_settings::NODE_GRAPH_SETTINGS]);

	__PARSE_SUCESS__(l_status)

	return true;
}

bool ProjectSaver::parseMaterialSettings(const Json::Value& p_value) {
	bool l_status = true;

	l_status = ProjectSaver::parseMaterials(p_value);

	__PARSE_SUCESS__(l_status)

	return true;
}

bool ProjectSaver::parseMaterials(const Json::Value& p_value) {

	__IS_MEMBER_CHECK__(p_value, save_project::material_settings::MATERIAL_SETTINGS)

	Json::Value l_materialSettings = p_value[save_project::material_settings::MATERIAL_SETTINGS];

	__IS_MEMBER_CHECK__(l_materialSettings, save_project::material_settings::MATERIAL_ID)

	Json::Value l_materialIDs = l_materialSettings[save_project::material_settings::MATERIAL_ID];

	if (!l_materialIDs.isArray()) {
		return false;
	}

	bool l_status = true;
	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
	for (int i = 0; i < l_materialIDs.size(); i++) {
		if (!l_materialIDs[i].isInt()) {
			return false;
		}

		int l_ID = l_materialIDs[i].asInt();

		__IS_MEMBER_CHECK__(l_materialSettings, std::to_string(l_ID));

		l_status = ProjectSaver::parseMaterial(l_materialSettings[std::to_string(l_ID)], l_ID);

		__PARSE_SUCESS__(l_status)
	}

	return true;
}

bool ProjectSaver::parseMaterial(const Json::Value& p_value, int p_ID) {

	__IS_MEMBER_CHECK__(p_value, save_project::material_settings::MATERIAL_TYPE)
	__IS_MEMBER_CHECK__(p_value, save_project::material_settings::COLOR)

	if (!p_value[save_project::material_settings::MATERIAL_TYPE].isString()) {
			return false;
	}

	std::string l_materialType = p_value[save_project::material_settings::MATERIAL_TYPE].asString();

	bool l_status = true;
	Material* l_material = new Material();

	if (l_materialType == material_type::CUSTOM) {
		__IS_MEMBER_CHECK__(p_value, save_project::material_settings::FILE_NAME)
		__IS_MEMBER_CHECK__(p_value, save_project::material_settings::CUSTOM_NAME)

		if (!p_value[save_project::material_settings::FILE_NAME].isString()) {
				return false;
		}

		std::string l_filePath = save_project::CUSTOM_MATERIAL_FILE_PATH + p_value[save_project::material_settings::FILE_NAME].asString();

		if (!p_value[save_project::material_settings::CUSTOM_NAME].isString()) {
			return false;
		}

		std::string l_customName = p_value[save_project::material_settings::CUSTOM_NAME].asString();

		CustomMaterial::AddCustomMaterialAtFilePath(l_filePath);
		CustomMaterial* l_customMaterial = new CustomMaterial(l_customName, p_ID);

		if (l_customMaterial->isMalformed()) {
			return false;
		}

		l_material = l_customMaterial;
		
	}
	else {
		l_material = ProjectSaver::getMaterialFromName(l_materialType, p_ID);
	}

	float l_color[3];

	l_status = ProjectSaver::parseColor(p_value[save_project::material_settings::COLOR], l_color);

	__PARSE_SUCESS__(l_status)

	l_material->setColor(l_color);

	if (l_material->getFloatLabelsSize() > 0 && (!p_value.isMember(save_project::material_settings::INPUT_FLOAT) ||
		!p_value[save_project::material_settings::INPUT_FLOAT].isArray())) {
		return false;
	}

	Json::Value l_inputFloat = p_value[save_project::material_settings::INPUT_FLOAT];

	if (l_inputFloat.size() != l_material->getFloatLabelsSize()) {
		return false;
	}

	std::vector<float> l_inputFloats;

	for (int i = 0; i < l_inputFloat.size(); i++) {
		if (!l_inputFloat[i].isDouble()) {
				return false;
		}

		l_inputFloats.push_back(l_inputFloat[i].asFloat());
	}

	l_material->setInputFloats(l_inputFloats);

	if (l_material->getFloat3LabelsSize() > 0 && (!p_value.isMember(save_project::material_settings::INPUT_FLOAT3) ||
		!p_value[save_project::material_settings::INPUT_FLOAT3].isArray())) {
		return false;
	}

	Json::Value l_inputFloat3 = p_value[save_project::material_settings::INPUT_FLOAT3];

	if (l_inputFloat3.size() != l_material->getFloat3LabelsSize()) {
		return false;
	}

	std::vector<std::vector<float>> l_inputFloat3s;

	for (int i = 0; i < l_inputFloat3.size(); i++) {
		if (!l_inputFloat3[i].isArray() || l_inputFloat3[i].size() != 3) {
			return false;
		}

		std::vector<float> l_float3params;

		for (int j = 0; j < 3; j++) {
			if (!l_inputFloat3[i][j].isDouble()) {
				return false;
			}

			l_float3params.push_back(l_inputFloat3[i][j].asFloat());
		}

		l_inputFloat3s.push_back(l_float3params);
	}

	l_material->setInputFloat3s(l_inputFloat3s);

	NodeGraph::getSingleton()->addMaterial(l_material);

	return true;
}

bool ProjectSaver::parseNodeLink(const Json::Value& p_value) {

	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
	__IS_MEMBER_CHECK__(p_value, save_project::node_graph_settings::NODE_LINK)

	Json::Value l_nodeLink = p_value[save_project::node_graph_settings::NODE_LINK];
	if (!l_nodeLink.isArray()) {
		return false;
	}
	for (int i = 0; i < l_nodeLink.size(); i++) {
		if (!l_nodeLink[i].isArray() && l_nodeLink[i].size() != 2) {
			return false;
		}

		l_nodeGraph->m_links.push_back(std::make_pair(l_nodeLink[i][0].asInt(), l_nodeLink[i][1].asInt()));
	}
	return true;

}

bool ProjectSaver::parseNodes(const Json::Value& p_value) {

	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
	
	__IS_MEMBER_CHECK__(p_value, save_project::node_graph_settings::NODE_ID)

	Json::Value l_nodeIDList = p_value[save_project::node_graph_settings::NODE_ID];

	if (!l_nodeIDList.isArray()) {
		return false;
	}

	bool l_status = true;
	std::string l_fileContent;
	for (int i = 0; i < l_nodeIDList.size(); i++) {
		if (!l_nodeIDList[i].isInt()) {
			return false;
		}

		int l_ID = l_nodeIDList[i].asInt();

		__IS_MEMBER_CHECK__(p_value, std::to_string(l_ID))

		if (p_value[std::to_string(l_ID)].isMember(save_project::node_graph_settings::FILE_NAME)) {
			__IS_MEMBER_CHECK__(p_value, save_project::node_graph_settings::CUSTOM_FILE_CONTENT)

			Json::Value l_customFileContent = p_value[save_project::node_graph_settings::CUSTOM_FILE_CONTENT];
			std::string l_fileName = p_value[std::to_string(l_ID)][save_project::node_graph_settings::FILE_NAME].asString();
			
			if (!l_customFileContent[l_fileName].isString()) {
				return false;
			}
			l_fileContent = l_customFileContent[l_fileName].asString();
		}

		l_status = ProjectSaver::parseNode(p_value[std::to_string(l_ID)], l_ID, l_fileContent);
		__PARSE_SUCESS__(l_status);
	}

	return true;
}

bool ProjectSaver::parseNode(const Json::Value& p_value, int p_ID, std::string p_fileContent) {

	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
	bool l_isCustomNode = false;
	Node* l_node = new Node();

	if (p_value.isMember(save_project::node_graph_settings::FILE_NAME)) {
		l_isCustomNode = true;
	}

	__IS_MEMBER_CHECK__(p_value, save_project::node_graph_settings::NODE_NAME)

	if (!p_value[save_project::node_graph_settings::NODE_NAME].isString()) {
			return false;
	}

	std::string l_nodeName = p_value[save_project::node_graph_settings::NODE_NAME].asString();

	if (!l_isCustomNode) {
		l_node = ProjectSaver::getNodeFromNodeName(l_nodeName, p_ID);
	}
	else {
		std::string l_fileName = save_project::CUSTOM_SDF_FILE_PATH + p_value[save_project::node_graph_settings::FILE_NAME].asString();
		CustomNode::AddCustomNodeAtFilePath(l_fileName, p_fileContent);
		CustomNode* l_customNode = new CustomNode(l_nodeName, p_ID);
		if (l_customNode->isMalformed()) {
			delete(l_customNode);
			return false;
		}

		l_node = l_customNode;
	}

	// populate input pin IDs
	if (l_node->m_inputPins.size() > 0 && (!p_value.isMember(save_project::node_graph_settings::INPUT_IDS) ||
		!p_value[save_project::node_graph_settings::INPUT_IDS].isArray())) {
		return false;
	}

	for (int i = 0; i < l_node->m_inputPins.size(); i++) {
		if (!p_value[save_project::node_graph_settings::INPUT_IDS][i].isInt()) {
			return false;
		}
		l_node->m_inputIDs[i] = p_value[save_project::node_graph_settings::INPUT_IDS][i].asInt();
		l_nodeGraph->setID(l_node, l_node->m_inputIDs[i]);
	}

	// populate output pin IDs
	if (l_node->m_outputPins.size() > 0 && (!p_value.isMember(save_project::node_graph_settings::OUTPUT_IDS) ||
		!p_value[save_project::node_graph_settings::OUTPUT_IDS].isArray())) {
		return false;
	}
	for (int i = 0; i < l_node->m_outputIDs.size(); i++) {
		if (!p_value[save_project::node_graph_settings::OUTPUT_IDS][i].isInt()) {
			return false;
		}
		l_node->m_outputIDs[i] = p_value[save_project::node_graph_settings::OUTPUT_IDS][i].asInt();
		l_nodeGraph->setID(l_node, l_node->m_outputIDs[i]);
	}

	// populate float inputs params
	if (l_node->m_inputFloatLabels.size() > 0 && (!p_value.isMember(save_project::node_graph_settings::INPUT_FLOAT) ||
		!p_value[save_project::node_graph_settings::INPUT_FLOAT].isArray())) {
		return false;
	}

	if (l_node->m_inputFloatLabels.size() != p_value[save_project::node_graph_settings::INPUT_FLOAT].size()) {
		return false;
	}
	for (int i = 0; i < l_node->m_inputFloatLabels.size(); i++) {
		if (!p_value[save_project::node_graph_settings::INPUT_FLOAT][i].isDouble()) {
			return false;
		}
		l_node->m_inputFloats[i] = p_value[save_project::node_graph_settings::INPUT_FLOAT][i].asDouble();
	}

	// populate float3 inputs params
	if (l_node->m_inputFloat3Labels.size() > 0 && (!p_value.isMember(save_project::node_graph_settings::INPUT_FLOAT3)
		|| !p_value[save_project::node_graph_settings::INPUT_FLOAT3].isArray())) {
		return false;
	}

	if (l_node->m_inputFloat3Labels.size() != p_value[save_project::node_graph_settings::INPUT_FLOAT3].size()) {
		return false;
	}

	for (int i = 0; i < l_node->m_inputFloat3Labels.size(); i++) {
		if (!p_value[save_project::node_graph_settings::INPUT_FLOAT3][i].isArray() || 
			!(p_value[save_project::node_graph_settings::INPUT_FLOAT3][i].size() == 3)) {
			return false;
		}

		for (int j = 0; j < 3; j++) {
			if (!p_value[save_project::node_graph_settings::INPUT_FLOAT3][i][j].isDouble()) {
				return false;
			}
			l_node->m_inputFloat3[i][j] = p_value[save_project::node_graph_settings::INPUT_FLOAT3][i][j].asDouble();
		}

	}

	if (l_node->m_isObjectNode) {
		ObjectNode* l_objectNode = (ObjectNode*)l_node;
		__IS_MEMBER_CHECK__(p_value, save_project::node_graph_settings::MATERIAL_ID)

		l_objectNode->setMaterialID(std::stoi(p_value[save_project::node_graph_settings::MATERIAL_ID].asString()));
	}

	l_nodeGraph->addNode(l_node);

	return true;

}

bool ProjectSaver::parseRenderingSettings(const Json::Value& p_value) {
	
	Renderer* l_renderer = Renderer::getSingleton();
	
	__IS_MEMBER_CHECK__(p_value, save_project::render_settings::RENDER_SETTINGS)

	Json::Value l_renderingSettings = p_value[save_project::render_settings::RENDER_SETTINGS];

	__IS_MEMBER_CHECK__(l_renderingSettings, save_project::render_settings::RENDER_DEBUG_VALUES)

	Json::Value l_renderValues = l_renderingSettings[save_project::render_settings::RENDER_VALUES];

	if (!l_renderValues.isArray() || l_renderValues.size() != l_renderer->m_render_uniforms_values.size()) {
		return false;
	}

	for (int i = 0; i < l_renderValues.size(); i++) {
		if (!l_renderValues[i].isInt()) {
			return false;
		}

		l_renderer->m_render_uniforms_values[i] = l_renderValues[i].asInt();
	}

	__IS_MEMBER_CHECK__(l_renderingSettings, save_project::render_settings::RENDER_DEBUG_VALUES)

	Json::Value l_debugValues = l_renderingSettings[save_project::render_settings::RENDER_DEBUG_VALUES];

	if (!l_debugValues.isArray() || l_debugValues.size() != l_renderer->m_render_uniforms_debug_values.size()) {
		return false;
	}

	for (int i = 0; i < l_debugValues.size(); i++) {
		if (!l_debugValues[i].isBool()) {
			return false;
		}

		l_renderer->m_render_uniforms_debug_values[i] = l_debugValues[i].asBool();
	}

	__IS_MEMBER_CHECK__(l_renderingSettings, save_project::render_settings::RENDER_DEBUG_COLORS)

	Json::Value l_debugColor = l_renderingSettings[save_project::render_settings::RENDER_DEBUG_COLORS];

	if (!l_debugColor.isArray() || l_debugColor.size() != l_renderer->m_render_uniform_debug_cols.size()) {
		return false;
	}

	bool l_status = true;
	for (int i = 0; i < l_debugColor.size(); i++) {
		if (!l_debugColor[i].isArray()) {
			return false;
		}

		std::vector<float> l_color(3);
		l_status = ProjectSaver::parseColor(l_debugColor[i], l_color);

		__PARSE_SUCESS__(l_status);

		l_renderer->m_render_uniform_debug_cols[i] = l_color;
	}

	return true;
}

bool ProjectSaver::parseWorldSettings(const Json::Value& p_value) {

	__IS_MEMBER_CHECK__(p_value, save_project::world_settings::WORLD_SETTINGS)

	Json::Value l_worldSettings = p_value[save_project::world_settings::WORLD_SETTINGS];
	Renderer* l_renderer = Renderer::getSingleton();
	bool l_status = true;

	__IS_MEMBER_CHECK__(l_worldSettings, save_project::world_settings::CAMERA_ORIGIN)

	Json::Value l_cameraOrigin = l_worldSettings[save_project::world_settings::CAMERA_ORIGIN];

	if (!l_cameraOrigin.isArray() || l_cameraOrigin.size() != 3) {
		return false;
	}

	float l_cameraParams[3];
	for (int i = 0; i < l_cameraOrigin.size(); i++) {
		if (!l_cameraOrigin[i].isDouble()) {
			return false;
		}

		l_cameraParams[i] = l_cameraOrigin[i].asFloat();
	}

	l_renderer->setCameraOrigin(l_cameraParams);

	__IS_MEMBER_CHECK__(l_worldSettings, save_project::world_settings::CAMERA_FOCAL_LENGTH)

	Json::Value l_focalLength = l_worldSettings[save_project::world_settings::CAMERA_FOCAL_LENGTH];

	if (!l_focalLength.isDouble()) {
		return false;
	}

	l_renderer->setFocalLength(l_focalLength.asFloat());

	__IS_MEMBER_CHECK__(l_worldSettings, save_project::world_settings::TOP_COLOR)

	float l_topColor[3];
	l_status = ProjectSaver::parseColor(l_worldSettings[save_project::world_settings::TOP_COLOR], l_topColor);

	__PARSE_SUCESS__(l_status)

	for (int i = 0; i < 3; i++) {
		l_renderer->m_horizon_top_color[i] = l_topColor[i];
	}

	__IS_MEMBER_CHECK__(l_worldSettings, save_project::world_settings::BOTTOM_COLOR)

	float l_bottomColor[3];
	l_status = ProjectSaver::parseColor(l_worldSettings[save_project::world_settings::BOTTOM_COLOR], l_bottomColor);

	__PARSE_SUCESS__(l_status);

	for (int i = 0; i < 3; i++) {
		l_renderer->m_horizon_bottom_color[i] = l_bottomColor[i];
	}

	return true;
}

template<typename T>
bool ProjectSaver::parseColor(const Json::Value& p_value, T &p_color) {

	if (!p_value.isArray() || p_value.size() != 3) {
		return false;
	}

	for (int i = 0; i < 3; i++) {
		if (!p_value[i].isDouble() || p_value[i].asFloat() < 0.0 || p_value[i].asFloat() > 1.0) {
			return false;
		}

		p_color[i] = p_value[i].asFloat();
	}

	return true;
}