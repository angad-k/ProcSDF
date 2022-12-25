#pragma once
#include <vector>
#include "GUI/Nodes/Node.h"
#include "Common/custom.h"
class CustomNode : public Node {
	bool m_isMalformed = false;
public:
	CustomNode(std::string p_nodeName);
	bool isMalformed()
	{
		return m_isMalformed;
	}
	static void AddCustomNodeAtFilePath(std::string p_filePath);
};