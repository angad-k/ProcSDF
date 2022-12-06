#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
class PrimitiveNode : public Node {
public:
	PrimitiveNode()
	{
		m_titleColor = imgui_colors::RED;
		m_inputPins = {};
		m_outputPins = { "Output" };
		m_isFinalNode = false;
	}
};
class SphereNode : public PrimitiveNode {
public:
	SphereNode()
	{
		m_nodeName = "Sphere";
		m_inputFloat3Labels = { };
		m_inputFloatLabels = { "Radius" };
		init();
	}
	SphereNode(int p_ID)
	{
		m_nodeName = "Sphere";
		m_inputFloat3Labels = { };
		m_inputFloatLabels = { "Radius" };
		init(p_ID);
	}
};

class BoxNode : public PrimitiveNode {
public:
	BoxNode()
	{
		m_nodeName = "Box";
		m_inputFloat3Labels = { "Box Dimension" };
		m_inputFloatLabels = {};
		init();
	}
	BoxNode(int p_ID)
	{
		m_nodeName = "Box";
		m_inputFloat3Labels = { "Box Dimension" };
		m_inputFloatLabels = {};
		init(p_ID);
	}
};

class TorusNode : public PrimitiveNode {
public:
	TorusNode()
	{
		m_nodeName = "Torus";
		m_inputFloat3Labels = {};
		m_inputFloatLabels = { "Path Radius", "Circle Radius" };
		init();
	}
	TorusNode(int p_ID)
	{
		m_nodeName = "Torus";
		m_inputFloat3Labels = {};
		m_inputFloatLabels = { "Path Radius", "Circle Radius" };
		init(p_ID);
	}
};

class BoxFrameNode : public PrimitiveNode {
public:
	BoxFrameNode()
	{
		m_nodeName = "BoxFrame";
		m_inputFloat3Labels = { "Box Dimension" };
		m_inputFloatLabels = { "Frame Thickness" };
		init();
	}
	BoxFrameNode(int p_ID)
	{
		m_nodeName = "BoxFrame";
		m_inputFloat3Labels = { "Box Dimension" };
		m_inputFloatLabels = { "Frame Thickness" };
		init(p_ID);
	}
};

