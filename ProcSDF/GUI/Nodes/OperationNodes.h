#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
class OperationNode : public Node {
public:
	OperationNode()
	{
		m_titleColor = imgui_colors::BLUE;
	}
};
class IntersectionNode : public OperationNode {
public:
	IntersectionNode()
	{
		m_nodeName = "Intersection";
		m_inputPins = { "Input 1", "Input 2" };
		m_outputPins = { "Output" };
		init();
	}
	IntersectionNode(int p_ID)
	{
		m_nodeName = "Intersection";
		m_inputPins = { "Input 1", "Input 2" };
		m_outputPins = { "Output" };
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
		init();
	}
	UnionNode(int p_ID)
	{
		m_nodeName = "Union";
		m_inputPins = { "Input 1", "Input 2" };
		m_outputPins = { "Output" };
		init(p_ID);
	}
};

class RoundNode : public OperationNode {
public:
	RoundNode()
	{
		m_nodeName = "Round";
		m_inputPins = { "Input" };
		m_inputFloatLabels = { "radius" };
		m_outputPins = { "Output" };
		init();
	}
	RoundNode(int p_ID)
	{
		m_nodeName = "Round";
		m_inputPins = { "Input" };
		m_inputFloatLabels = { "radius" };
		m_outputPins = { "Output" };
		init(p_ID);
	}
};