#pragma once
#include <vector>
#include "GUI/Nodes/Node.h"
#include "Common/custom.h"
class CustomNode : public Node {
	bool m_isMalformed = false;
	void parseCustomNodeFile(std::string p_nodeName);
	bool m_isTransformNode = false, m_isPrimitiveNode = false, m_isOperationNode = false;
public:
	bool isTransform()
	{
		return m_isTransformNode;
	}
	bool isOperation()
	{
		return m_isOperationNode;
	}
	bool isPrimitive()
	{
		return m_isPrimitiveNode;
	}
	CustomNode(std::string p_nodeName);
	CustomNode(std::string p_nodeName, int p_ID);
	bool isMalformed()
	{
		return m_isMalformed;
	}
	static void AddCustomNodeAtFilePath(std::string p_filePath, std::string p_fileContent = "");
};