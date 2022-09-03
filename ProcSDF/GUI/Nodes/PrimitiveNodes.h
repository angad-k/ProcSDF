#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
class SphereNode : public Node {
public:
	std::string get_string() { return ""; }
	SphereNode()
	{
		node_name = "Sphere node";
		input_pins = {};
		output_pins = { "Output" };
		is_final_node = false;
		init();
	}
};