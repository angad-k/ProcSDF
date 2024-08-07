#include "CustomNode.h"
#include "Common/os.h"
#include "GUI/NodeGraph.h"
#include <iostream>
#include <regex>
#include <string>
#include <vector>

CustomNode::CustomNode(std::string p_nodeName)
{
	CustomNode::parseCustomNodeFile(p_nodeName);
	init();
}

CustomNode::CustomNode(std::string p_nodeName, int m_ID)
{
	CustomNode::parseCustomNodeFile(p_nodeName);
	init(m_ID);
}

void CustomNode::parseCustomNodeFile(std::string p_nodeName) {
	std::string l_fileContent = NodeGraph::getSingleton()->getCustomNodeFileContentsfromNodeName(p_nodeName);
	std::vector<std::string> l_lines = custom::tokenizeWithDelimiters(l_fileContent, "\n");
	m_titleColor = imgui_colors::CUSTOM_NODE;
	for (std::string line : l_lines)
	{
		std::vector<std::string> l_tokens = custom::tokenizeWithDelimiters(line, "\\s+");

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
					m_isPrimitiveNode = true;
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
}

void CustomNode::AddCustomNodeAtFilePath(std::string p_filePath, std::string p_fileContent)
{
	std::string l_fileContent;
	if (p_fileContent == "") {
		l_fileContent = OS::fetchFileContent(p_filePath);
	}
	else {
		l_fileContent = p_fileContent;
	}
	std::vector<std::string> l_lines = custom::tokenizeWithDelimiters(l_fileContent, "\n");
	bool l_isMalformed = false;
	std::string l_nodeName = "";
	for (std::string line : l_lines)
	{
		std::vector<std::string> l_tokens = custom::tokenizeWithDelimiters(line, "\\s+");

		if (l_tokens[0] == "//")
		{
			if (l_tokens[1] == "type")
			{
				if (l_tokens.size() <= 2)
				{
					ERR("type should be followed by an argument.");
					l_isMalformed = true;
					break;
				}
			}
			else if (l_tokens[1] == "name")
			{
				if (l_tokens.size() <= 2)
				{
					ERR("type should be followed by an argument.");
					l_isMalformed = true;
					break;
				}
				l_nodeName = l_tokens[2];
			}
		}
	}
	if (!l_isMalformed)
	{
		NodeGraph::getSingleton()->setCustomNodeFileContents(l_nodeName, l_fileContent);
		NodeGraph::getSingleton()->setCustomNodeFilePath(l_nodeName, p_filePath);
	}
}