#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
class IntersectionNode : public Node {
public:
	std::string get_string() { return ""; }
	IntersectionNode()
	{
		node_name = "Intersection node";
		input_pins = { "Input 1", "Input 2" };
		output_pins = { "Output" };
		is_final_node = false;
		init();
	}
};