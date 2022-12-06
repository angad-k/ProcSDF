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

bool ProjectSaver::saveProject() {
	
	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
	Renderer* l_renderer = Renderer::getSingleton();
	Json::Value l_jsonValue;

	int index = 0;
	for (std::pair<int, int> link : l_nodeGraph->m_links) {
		l_jsonValue[save_project::NODE_LINK][index][0] = link.first;
		l_jsonValue[save_project::NODE_LINK][index][1] = link.second;
		index++;
	}

	// save camera details

	for (int i = 0; i < 3; i++) {
		l_jsonValue[save_project::CAMERA_SETTING][save_project::CAMERA_ORIGIN][i] = l_renderer->get_camera_origin()[i];
	}

	l_jsonValue[save_project::CAMERA_SETTING][save_project::CAMERA_FOCAL_LENGTH] = *(l_renderer->getFocalLength());

	// save minimal details for each node
	/*
	  "$(m_ID) : {
	   "nodeName" : $(m_nodeName),
	   "inputIDs" : $(m_inputIDs),
	   "outputIDS": $(m_outputIDs),
	   "inputFloat3" : $(m_inputFloat3),
	   "inputFloat" : $(m_inputFloat)
	  }
	*/

	index = 0;
	for (Node* nd : l_nodeGraph->m_nodes) {
		
		l_jsonValue[std::to_string(nd->m_ID)][save_project::NODE_NAME] = nd->m_nodeName;
		l_jsonValue[save_project::NODE_ID][index] = nd->m_ID;

		for (int i = 0; i < nd->m_inputIDs.size(); i++) {
			l_jsonValue[std::to_string(nd->m_ID)][save_project::INPUT_IDS][i] = nd->m_inputIDs[i];
		}

		for (int i = 0; i < nd->m_outputIDs.size(); i++) {
			l_jsonValue[std::to_string(nd->m_ID)][save_project::OUTPUT_IDS][i] = nd->m_outputIDs[i];
		}

		for (int i = 0; i < nd->m_inputFloat3.size(); i++) {
			l_jsonValue[std::to_string(nd->m_ID)][save_project::INPUT_FLOAT3][i][0] = nd->m_inputFloat3[i][0];
			l_jsonValue[std::to_string(nd->m_ID)][save_project::INPUT_FLOAT3][i][1] = nd->m_inputFloat3[i][1];
			l_jsonValue[std::to_string(nd->m_ID)][save_project::INPUT_FLOAT3][i][2] = nd->m_inputFloat3[i][2];
		}

		for (int i = 0; i < nd->m_inputFloats.size(); i++) {
			l_jsonValue[std::to_string(nd->m_ID)][save_project::INPUT_FLOAT][i] = nd->m_inputFloats[i];
		}

		index++;

	}

	Json::StyledWriter styledWriter;
	Json::FastWriter fastWriter;

	std::cout << styledWriter.write(l_jsonValue) << "\n";

	std::pair<bool, std::string> l_filePathInfo = OS::pickSaveAsFile();
	
	if(l_filePathInfo.first) {
	    bool status = OS::saveFileContent(l_filePathInfo.second, std::string(styledWriter.write(l_jsonValue)));
		return status;
	}

	return false;
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
		l_node == new RotationNodeZ(p_ID);
	}

	return l_node;
}

bool ProjectSaver::loadProject() {
	std::pair<bool, std::string> l_filePath = OS::pickFile();

	if (!l_filePath.first) {
		return false;
	}

	// try catch included here only for safety in case any edge slips by
	try {
		std::string l_jsonString = OS::fetchFileContent(l_filePath.second);

		NodeGraph::getSingleton()->clear();
		NodeGraph* l_nodeGraph = NodeGraph::getSingleton();

		Json::Value l_value, l_nodeIDList, l_nodeInfo;
		Json::Reader l_reader;
		bool l_isParseSucessful = l_reader.parse(l_jsonString, l_value);
		if (!l_isParseSucessful || !l_value.isMember(save_project::NODE_ID) || !l_value[save_project::NODE_ID].isArray()) {
			return false;
		}
		
		l_nodeIDList = l_value[save_project::NODE_ID];

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
					l_node->m_inputFloat3[i][j] = l_nodeInfo[save_project::INPUT_FLOAT][i][j].asDouble();
				}
				
			}

		}
	}
	catch (...) {
		return false;
	}

	return true;
}