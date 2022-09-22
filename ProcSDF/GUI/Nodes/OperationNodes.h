#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
class OperationNode : public Node {
public:
	OperationNode()
	{
		title_color = imgui_colors::BLUE;
		input_pins = { "Input 1", "Input 2" };
		output_pins = { "Output" };
		is_final_node = false;
	}
};
class IntersectionNode : public OperationNode {
public:
	std::string get_string() { return ""; }
	IntersectionNode()
	{
		node_name = "Intersection Node";
		init();
	}
};