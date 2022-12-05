#include "Common/ProjectSaver.h"
#include "GUI/NodeGraph.h"
#include "Common/constant.h"
#include "Common/WindowsInterface.h"
#include "Common/os.h"

bool ProjectSaver::saveStringToFile(std::string p_filePath) {
	return true;
}

bool ProjectSaver::saveProject(std::string p_filePath) {
	
	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
	Json::Value l_jsonValue;

	int index = 0;
	for (std::pair<int, int> link : l_nodeGraph->m_links) {
		l_jsonValue[save_project::NODE_LINK][index][0] = link.first;
		l_jsonValue[save_project::NODE_LINK][index][1] = link.second;
		index++;
	}

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

	for (Node* nd : l_nodeGraph->m_nodes) {
		l_jsonValue[std::to_string(nd->m_ID)][save_project::NODE_NAME] = nd->m_nodeName;

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

	}

	Json::StyledWriter styledWriter;
	Json::FastWriter fastWriter;

	std::cout << styledWriter.write(l_jsonValue) << "\n";

	std::pair<bool, std::string> l_filePathInfo = WindowsInterface::saveFile("ProcSDF Node Space (*.procsdf)\0*.procsdf\0");
	
	if(l_filePathInfo.first) {
	    bool flag =	OS::saveFileContent(l_filePathInfo.second + save_project::PROCSDF_EXTENSION, std::string(styledWriter.write(l_jsonValue)));
	}

	return true;
}