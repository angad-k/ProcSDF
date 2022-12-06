#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
class OperationNode : public Node {
public:
	OperationNode()
	{
		m_titleColor = imgui_colors::BLUE;
		m_isOperationNode = true;
	}
};
class IntersectionNode : public OperationNode {
public:
	IntersectionNode()
	{
		m_nodeName = "Intersection";
		m_inputPins = { "Input 1", "Input 2" };
		m_outputPins = { "Output" };
		m_isFinalNode = false;
		init();
	}
	IntersectionNode(int p_ID)
	{
		m_nodeName = "Intersection";
		m_inputPins = { "Input 1", "Input 2" };
		m_outputPins = { "Output" };
		m_isFinalNode = false;
		init(p_ID);
	}
};

class UnionNode : public OperationNode {
public:
	UnionNode()
	{
		m_nodeName = "Union";
		m_inputPins = { "Input 1", "Input 2" };
		m_outputPins = { "Output" };
		m_isFinalNode = false;
		init();
	}
	UnionNode(int p_ID)
	{
		m_nodeName = "Union";
		m_inputPins = { "Input 1", "Input 2" };
		m_outputPins = { "Output" };
		m_isFinalNode = false;
		init(p_ID);
	}
};
