#include "CustomNode.h"
#include "Common/os.h"
#include "GUI/NodeGraph.h"
#include <iostream>
#include <regex>
#include <string>
#include <vector>
std::vector<std::string> CustomNode::tokenizeWithDelimiters(std::string p_string, std::string p_delimiter)
{
	std::regex l_reg(p_delimiter);

	std::sregex_token_iterator l_iter(p_string.begin(), p_string.end(), l_reg, -1);
	std::sregex_token_iterator l_end;

	std::vector<std::string> l_tokens(l_iter, l_end);

	return l_tokens;
}

CustomNode::CustomNode(std::string p_filePath)
{
	std::string l_fileContent = OS::fetchFileContent(p_filePath);
	std::vector<std::string> l_lines = tokenizeWithDelimiters(l_fileContent, "\n");
	for (std::string line : l_lines)
	{
		std::vector<std::string> l_tokens = tokenizeWithDelimiters(line, "\\s+");

		if (l_tokens[0] == "//")
		{
			if (l_tokens[1] == "type")
			{
				if (l_tokens.size() <= 2)
				{
					ERR("type should be followed by an argument.");
					m_isMalformed = true;
					break;
				}
				if (l_tokens[2] == "primitive")
				{
					// do nothing for now.
				}
				if (l_tokens[2] == "operation")
				{
					m_isOperationNode = true;
				}
				if (l_tokens[2] == "transform")
				{
					m_isTransformNode = true;
				}
			}
			else if (l_tokens[1] == "name")
			{
				if (l_tokens.size() <= 2)
				{
					ERR("type should be followed by an argument.");
					m_isMalformed = true;
					break;
				}
				m_nodeName = l_tokens[2];
			}
			else if (l_tokens[1] == "input_pins")
			{
				m_inputPins.insert(m_inputPins.end(), l_tokens.begin() + 2, l_tokens.end());
			}
			else if (l_tokens[1] == "vec3_params")
			{
				m_inputFloat3Labels.insert(m_inputFloat3Labels.end(), l_tokens.begin() + 2, l_tokens.end());
			}
			else if (l_tokens[1] == "float_params")
			{
				m_inputFloatLabels.insert(m_inputFloatLabels.end(), l_tokens.begin() + 2, l_tokens.end());
			}
		}
	}
	m_outputPins.push_back("Output");

	NodeGraph::getSingleton()->setCustomNodeFileContents(p_filePath, l_fileContent);

	init();
}