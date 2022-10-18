#pragma once
#include "GUI/Nodes/Node.h"
#include "GUI/NodeGraph.h"

class TransformNode : public Node {
public:
	TransformNode()
	{
		is_tranform_node = true;
		input_pins = { "Input" };
		output_pins = { "Output" };
		title_color = imgui_colors::ORANGE;
	}
};

class TranslationNode : public TransformNode {
public:
	TranslationNode()
	{
		node_name = "Translation";
		input_float3_labels = { "Translation" };
		init();
	}
};

class RotationNodeX : public TransformNode {
public:
	RotationNodeX()
	{
		node_name = "Rotation Around X-Axis";
		input_float_labels = { "Theta" };
		init();
	}
};

class RotationNodeY : public TransformNode {
public:
	RotationNodeY()
	{
		node_name = "Rotation Around Y-Axis";
		input_float_labels = { "Theta" };
		init();
	}
};

class RotationNodeZ : public TransformNode {
public:
	RotationNodeZ()
	{
		node_name = "Rotation Around Z-Axis";
		input_float_labels = { "Theta" };
		init();
	}
};
