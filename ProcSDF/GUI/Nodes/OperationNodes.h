#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
class OperationNode : public Node {
public:
	OperationNode()
	{
		title_color = imgui_colors::BLUE;
		is_operation_node = true;
	}
};
class IntersectionNode : public OperationNode {
public:
	IntersectionNode()
	{
		node_name = "Intersection";
		input_pins = { "Input 1", "Input 2" };
		output_pins = { "Output" };
		is_final_node = false;
		init();
	}
};

class UnionNode : public OperationNode {
public:
	UnionNode()
	{
		node_name = "Union";
		input_pins = { "Input 1", "Input 2" };
		output_pins = { "Output" };
		is_final_node = false;
		init();
	}
};
