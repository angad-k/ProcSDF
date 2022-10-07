#pragma once
#include "GUI/Nodes/Node.h"
#include "GUI/NodeGraph.h"

class TransformNode : public Node {
public:
	TransformNode()
	{
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
