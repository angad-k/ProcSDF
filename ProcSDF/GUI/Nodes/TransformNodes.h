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
		is_transform_node = true;
		input_pins = { "Input" };
		output_pins = { "Output" };
		title_color = imgui_colors::ORANGE;
	}
};

class TranslationNode : public TransformNode {
public:
	TranslationNode()
	{
		m_TransformationType = TransformationType::TRANSLATION;
		node_name = "Translation";
		input_float3_labels = { "Translation" };
		init();
	}
};

class RotationNodeX : public TransformNode {
public:
	RotationNodeX()
	{
		m_TransformationType = TransformationType::ROTATION_X;
		node_name = "Rotation_X";
		input_float_labels = { "Theta" };
		init();
	}
};

class RotationNodeY : public TransformNode {
public:
	RotationNodeY()
	{
		m_TransformationType = TransformationType::ROTATION_Y;
		node_name = "Rotation_Y";
		input_float_labels = { "Theta" };
		init();
	}
};

class RotationNodeZ : public TransformNode {
public:
	RotationNodeZ()
	{
		m_TransformationType = TransformationType::ROTATION_Z;
		node_name = "Rotation_Z";
		input_float_labels = { "Theta" };
		init();
	}
};
