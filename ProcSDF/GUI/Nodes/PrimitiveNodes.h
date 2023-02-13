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

class RoundBoxNode : public PrimitiveNode {
public:
	RoundBoxNode()
	{
		m_nodeName = "RoundBox";
		m_inputFloat3Labels = { "Box Dimension" };
		m_inputFloatLabels = { "Radius" };
		init();
	}
	RoundBoxNode(int p_ID)
	{
		m_nodeName = "BoxFrame";
		m_inputFloat3Labels = { "Box Dimension" };
		m_inputFloatLabels = { "Radius" };
		init(p_ID);
	}
};

class ConeNode : public PrimitiveNode {
public:
	ConeNode()
	{
		m_nodeName = "Cone";
		m_inputFloatLabels = { "sin", "cos", "height"};
		init();
	}
	ConeNode(int p_ID)
	{
		m_nodeName = "Cone";
		m_inputFloatLabels = { "sin", "cos", "height" };
		init(p_ID);
	}
};

class CapsuleNode : public PrimitiveNode {
public:
	CapsuleNode()
	{
		m_nodeName = "Capsule";
		m_inputFloat3Labels = { "Point1", "Point2"};
		m_inputFloatLabels = { "radius" };
		init();
	}
	CapsuleNode(int p_ID)
	{
		m_nodeName = "Capsule";
		m_inputFloat3Labels = { "Point1", "Point2" };
		m_inputFloatLabels = { "radius" };
		init(p_ID);
	}
};

class CylinderNode : public PrimitiveNode {
public:
	CylinderNode()
	{
		m_nodeName = "Cylinder";
		m_inputFloat3Labels = { "Point1", "Point2" };
		m_inputFloatLabels = { "radius" };
		init();
	}
	CylinderNode(int p_ID)
	{
		m_nodeName = "Cylinder";
		m_inputFloat3Labels = { "Point1", "Point2" };
		m_inputFloatLabels = { "radius" };
		init(p_ID);
	}
};


