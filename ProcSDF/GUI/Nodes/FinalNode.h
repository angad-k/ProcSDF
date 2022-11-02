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
		m_isFinalNode = true;
		m_titleColor = imgui_colors::GREEN;
		init();
	}
};
