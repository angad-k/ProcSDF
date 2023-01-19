#pragma once
#include <vector>
#include "GUI/Nodes/Node.h"
#include "Common/custom.h"
class CustomNode : public Node {
	bool m_isMalformed = false;
	void parseCustomNodeFile(std::string p_nodeName);
public:
	CustomNode(std::string p_nodeName);
	CustomNode(std::string p_nodeName, int p_ID);
	bool isMalformed()
	{
		return m_isMalformed;
	}
	static void AddCustomNodeAtFilePath(std::string p_filePath);
};