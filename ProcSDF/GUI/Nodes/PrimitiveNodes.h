#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
class PrimitiveNode : public Node {
public:
	PrimitiveNode()
	{
		title_color = imgui_colors::RED;
	}
};
class SphereNode : public PrimitiveNode {
public:
	SphereNode()
	{
		node_name = "Sphere";
		input_pins = {};
		output_pins = { "Output" };
		is_final_node = false;
		input_float3_labels = { "Centre Position" };
		input_float_labels = { "Radius" };
		init();
	}
};
