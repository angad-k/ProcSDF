#pragma once
#include "GUI/Nodes/Node.h"

class FinalNode : public Node {
public:
	std::string get_string() { return ""; }
	FinalNode()
	{
		node_name = "Final Node";
		input_pins = { "Final Output" };
		output_pins = {};
		is_final_node = true;
		title_color = imgui_colors::GREEN;
		init();
	}
};
