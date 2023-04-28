#pragma once
#include "GUI/Nodes/Node.h"

class FinalNode : public Node {
public:
	FinalNode()
	{
		m_variableName = "final";
		m_nodeName = "Final node";
		m_inputPins = { "Final Output" };
		m_outputPins = {};
		m_titleColor = imgui_colors::GREEN;
		init();
	}
	FinalNode(int p_ID)
	{
		m_variableName = "final";
		m_nodeName = "Final node";
		m_inputPins = { "Final Output" };
		m_outputPins = {};
		m_titleColor = imgui_colors::GREEN;
		init(p_ID);
	}
};
