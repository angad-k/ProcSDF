#pragma once
#include "GUI/Nodes/Node.h"
#include "GUI/NodeGraph.h"

class ObjectNode : public Node {
public:
	ObjectNode()
	{
		m_variableName = "object";
		m_nodeName = "Object node";
		m_inputPins = { "Input" };
		m_outputPins = { "Final Output" };
		m_isObjectNode = true;
		m_titleColor = imgui_colors::PURPLE;
		init();
	}
	ObjectNode(int p_ID)
	{
		m_variableName = "object";
		m_nodeName = "Object node";
		m_inputPins = { "Input" };
		m_outputPins = { "Final Output" };
		m_isObjectNode = true;
		m_titleColor = imgui_colors::PURPLE;
		init(p_ID);
	}
};
