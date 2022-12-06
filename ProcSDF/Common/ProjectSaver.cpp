#include "Common/ProjectSaver.h"
#include "GUI/NodeGraph.h"
#include "Common/constant.h"
#include "Common/WindowsInterface.h"
#include "Common/os.h"
#include "Rendering/Renderer.h"

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

bool ProjectSaver::syntaxChecker(std::string p_jsonString) {
	Json::Value value;
	Json::Reader reader;
	bool l_isParseSucessful = reader.parse(p_jsonString, value);
	if (!l_isParseSucessful) {
		return false;
	}

	if (!value.isMember(save_project::CAMERA_SETTING) || !value.isMember(save_project::NODE_ID) || !value.isMember(save_project::NODE_LINK)
		|| !value[save_project::NODE_ID].isArray()) {
		return false;
	}

	Json::Value l_nodeIDList = value[save_project::NODE_ID];

	for (int i = 0; i < l_nodeIDList.size(); i++) {
		if (!value.isMember(std::to_string(l_nodeIDList[i].asInt()))) {
			return false;
		}
	}
	return true;

}

bool ProjectSaver::loadProject() {
	std::pair<bool, std::string> l_filePath = OS::pickFile();

	if (!l_filePath.first) {
		return false;
	}

	std::string l_jsonString = OS::fetchFileContent(l_filePath.second);

	NodeGraph::getSingleton()->clear();
	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();


	return true;
}