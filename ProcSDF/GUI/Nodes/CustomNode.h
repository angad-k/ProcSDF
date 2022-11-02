#pragma once
#include <vector>
#include "GUI/Nodes/Node.h"
class CustomNode : public Node {
	std::vector<std::string> tokenizeWithDelimiters(std::string p_string, std::string p_delimiter);
public:
	CustomNode(std::string p_filePath);
};