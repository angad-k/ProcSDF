#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
class PrimitiveNode : public Node {
public:
	PrimitiveNode()
	{
		title_color = imgui_colors::RED;
		input_pins = {};
		output_pins = { "Output" };
		is_final_node = false;
	}
};
class SphereNode : public PrimitiveNode {
public:
	SphereNode()
	{
		node_name = "sphere";
		input_float3_labels = { "Centre Position" };
		input_float_labels = { "Radius" };
		init();
	}
};
class BoxNode : public PrimitiveNode {
public:
	BoxNode()
	{
		node_name = "Box";
		input_float3_labels = { "Box Dimension" };
		input_float_labels = {};
		init();
	}
};
class TorusNode : public PrimitiveNode {
public:
	TorusNode()
	{
		node_name = "Torus";
		input_float3_labels = {};
		input_float_labels = { "Path Radius", "Circle Radius" };
		init();
	}
};
