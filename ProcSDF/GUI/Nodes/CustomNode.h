#pragma once
#include <vector>
#include "GUI/Nodes/Node.h"
class CustomNode : public Node {
	std::vector<std::string> tokenizeWithDelimiters(std::string p_string, std::string p_delimiter);
	bool m_isMalformed = false;
public:
	CustomNode(std::string p_filePath);
	bool isMalformed()
	{
		return m_isMalformed;
	}
};