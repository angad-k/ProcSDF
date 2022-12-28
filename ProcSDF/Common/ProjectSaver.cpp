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
#include "Rendering/Materials/Material.h"

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

		ProjectSaver::saveNode(p_value[std::to_string(node->m_ID)], node, l_isCustom, l_filePath, l_nodeIDToMaterialID[node->m_ID]);
	}

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
	}
}

void ProjectSaver::saveMaterial(Json::Value& p_value, Material* p_material) {

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

bool ProjectSaver::loadProject() {
	/*
	std::pair<bool, std::string> l_filePath = OS::pickFile();

	if (!l_filePath.first) {
		return false;
	}

	// try catch included here only for safety in case any edge slips by
	try {
		std::string l_jsonString = OS::fetchFileContent(l_filePath.second);

		NodeGraph::getSingleton()->clear();
		NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
		Renderer* l_renderer = Renderer::getSingleton();

		Json::Value l_value, l_nodeIDList, l_nodeInfo, l_cameraDetails;
		Json::Reader l_reader;
		float l_cameraOrigin[3];
		bool l_isParseSucessful = l_reader.parse(l_jsonString, l_value);
		if (!l_isParseSucessful || !l_value.isMember(save_project::NODE_ID) || !l_value[save_project::NODE_ID].isArray()) {
			return false;
		}
		
		l_nodeIDList = l_value[save_project::NODE_ID];
		// restore camera details

		if (!l_value.isMember(save_project::CAMERA_SETTING) || !l_value[save_project::CAMERA_SETTING].isMember(save_project::CAMERA_ORIGIN)
			|| !l_value[save_project::CAMERA_SETTING].isMember(save_project::CAMERA_FOCAL_LENGTH)) {
			return false;
		}

		l_cameraDetails = l_value[save_project::CAMERA_SETTING];

		if (!l_cameraDetails[save_project::CAMERA_FOCAL_LENGTH].isDouble() || !l_cameraDetails[save_project::CAMERA_ORIGIN].isArray()
			|| !(l_cameraDetails[save_project::CAMERA_ORIGIN].size() == 3)) {
			return false;
		}

		l_renderer->setFocalLength(l_cameraDetails[save_project::CAMERA_FOCAL_LENGTH].asDouble());

		for (int i = 0; i < 3; i++) {
			if (!l_cameraDetails[save_project::CAMERA_ORIGIN][i].isDouble()) {
				return false;
			}

			l_cameraOrigin[i] = l_cameraDetails[save_project::CAMERA_ORIGIN][i].asDouble();
			
		}

		l_renderer->setCameraOrigin(l_cameraOrigin);

		// restore link details

		if (!l_value.isMember(save_project::NODE_LINK) || !l_value[save_project::NODE_LINK].isArray()) {
			return false;
		}

		for (int i = 0; i < l_value[save_project::NODE_LINK].size(); i++) {
			if (!l_value[save_project::NODE_LINK][i].isArray() || !(l_value[save_project::NODE_LINK][i].size() == 2)
				|| !l_value[save_project::NODE_LINK][i][0].isInt() || !l_value[save_project::NODE_LINK][i][1].isInt()) {
				return false;
			}
			l_nodeGraph->m_links.push_back(std::make_pair(l_value[save_project::NODE_LINK][i][0].asInt(), l_value[save_project::NODE_LINK][i][1].asInt()));
		}

		// restore node info 
		for (int i = 0; i < l_nodeIDList.size(); i++) {
			
			int l_nodeID = l_nodeIDList[i].asInt();
			
			if (!l_value.isMember(std::to_string(l_nodeID))) {
				return false;
			}

			l_nodeInfo = l_value[std::to_string(l_nodeID)];

			if (!l_nodeInfo.isMember(save_project::NODE_NAME) || !l_nodeInfo[save_project::NODE_NAME].isString()) {
				return false;
			}

			Node* l_node = ProjectSaver::getNodeFromNodeName(l_nodeInfo[save_project::NODE_NAME].asString(), l_nodeID);

			// populate input pin IDs
			if (l_node->m_inputPins.size() > 0 && (!l_nodeInfo.isMember(save_project::INPUT_IDS) || !l_nodeInfo[save_project::INPUT_IDS].isArray())) {
				return false;
			}

			for (int i = 0; i < l_node->m_inputPins.size(); i++) {
				if (!l_nodeInfo[save_project::INPUT_IDS][i].isInt()) {
					return false;
				}
				l_node->m_inputIDs[i] = l_nodeInfo[save_project::INPUT_IDS][i].asInt();
				l_nodeGraph->setID(l_node, l_node->m_inputIDs[i]);
			}

			// populate output pin IDs
			if (l_node->m_outputPins.size() > 0 && (!l_nodeInfo.isMember(save_project::OUTPUT_IDS) || !l_nodeInfo[save_project::OUTPUT_IDS].isArray())) {
				return false;
			}
			for (int i = 0; i < l_node->m_outputIDs.size(); i++) {
				if (!l_nodeInfo[save_project::OUTPUT_IDS][i].isInt()) {
					return false;
				}
				l_node->m_outputIDs[i] = l_nodeInfo[save_project::OUTPUT_IDS][i].asInt();
				l_nodeGraph->setID(l_node, l_node->m_outputIDs[i]);
			}

			// populate float inputs params
			if (l_node->m_inputFloatLabels.size() > 0 && (!l_nodeInfo.isMember(save_project::INPUT_FLOAT) || !l_nodeInfo[save_project::INPUT_FLOAT].isArray())) {
				return false;
			}
			for (int i = 0; i < l_node->m_inputFloatLabels.size(); i++) {
				if (!l_nodeInfo[save_project::INPUT_FLOAT][i].isDouble()) {
					return false;
				}
				l_node->m_inputFloats[i] = l_nodeInfo[save_project::INPUT_FLOAT][i].asDouble();
			}

			// populate float3 inputs params
			if (l_node->m_inputFloat3Labels.size() > 0 && (!l_nodeInfo.isMember(save_project::INPUT_FLOAT3) || !l_nodeInfo[save_project::INPUT_FLOAT3].isArray())) {
				return false;
			}
			for (int i = 0; i < l_node->m_inputFloat3Labels.size(); i++) {
				if (!l_nodeInfo[save_project::INPUT_FLOAT3][i].isArray() || !(l_nodeInfo[save_project::INPUT_FLOAT3][i].size() == 3)) {
					return false;
				}

				for (int j = 0; j < 3; j++) {
					if (!l_nodeInfo[save_project::INPUT_FLOAT3][i][j].isDouble()) {
						return false;
					}
					l_node->m_inputFloat3[i][j] = l_nodeInfo[save_project::INPUT_FLOAT3][i][j].asDouble();
				}
				
			}

			l_nodeGraph->addNode(l_node);
		}
		l_nodeGraph->recompileNodeGraph();
	}
	catch (...) {
		return false;
	}
	*/
	return true;
}