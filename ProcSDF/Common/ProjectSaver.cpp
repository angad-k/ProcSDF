#include "Common/ProjectSaver.h"
#include "GUI/NodeGraph.h"
#include "Common/constant.h"

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

	Json::StyledWriter styledWriter;
	Json::FastWriter fastWriter;

	std::cout << styledWriter.write(l_jsonValue) << "\n";
	std::cout << fastWriter.write(l_jsonValue) << "\n";
	return true;
}