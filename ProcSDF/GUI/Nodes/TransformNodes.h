#pragma once
#include "GUI/Nodes/Node.h"
#include "GUI/NodeGraph.h"

enum class TransformationType
{
	TRANSLATION,
	ROTATION_X,
	ROTATION_Y,
	ROTATION_Z
};

class TransformNode : public Node {
public:
	TransformationType m_TransformationType;
	TransformNode()
	{
		m_isTransformNode = true;
		m_inputPins = { "Input" };
		m_outputPins = { "Output" };
		m_titleColor = imgui_colors::ORANGE;
	}
};

class TranslationNode : public TransformNode {
public:
	TranslationNode()
	{
		m_TransformationType = TransformationType::TRANSLATION;
		m_nodeName = "Translation";
		m_inputFloat3Labels = { "Translation" };
		init();
	}
	TranslationNode(int p_ID)
	{
		m_TransformationType = TransformationType::TRANSLATION;
		m_nodeName = "Translation";
		m_inputFloat3Labels = { "Translation" };
		init(p_ID);
	}
};

class RotationNodeX : public TransformNode {
public:
	RotationNodeX()
	{
		m_TransformationType = TransformationType::ROTATION_X;
		m_nodeName = "Rotation_X";
		m_inputFloatLabels = { "Theta" };
		init();
	}
	RotationNodeX(int p_ID)
	{
		m_TransformationType = TransformationType::ROTATION_X;
		m_nodeName = "Rotation_X";
		m_inputFloatLabels = { "Theta" };
		init(p_ID);
	}
};

class RotationNodeY : public TransformNode {
public:
	RotationNodeY()
	{
		m_TransformationType = TransformationType::ROTATION_Y;
		m_nodeName = "Rotation_Y";
		m_inputFloatLabels = { "Theta" };
		init();
	}
	RotationNodeY(int p_ID)
	{
		m_TransformationType = TransformationType::ROTATION_Y;
		m_nodeName = "Rotation_Y";
		m_inputFloatLabels = { "Theta" };
		init(p_ID);
	}
};

class RotationNodeZ : public TransformNode {
public:
	RotationNodeZ()
	{
		m_TransformationType = TransformationType::ROTATION_Z;
		m_nodeName = "Rotation_Z";
		m_inputFloatLabels = { "Theta" };
		init();
	}
	RotationNodeZ(int p_ID)
	{
		m_TransformationType = TransformationType::ROTATION_Z;
		m_nodeName = "Rotation_Z";
		m_inputFloatLabels = { "Theta" };
		init(p_ID);
	}
};
