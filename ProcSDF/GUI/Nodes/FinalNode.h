#pragma once
#include "GUI/Nodes/Node.h"

class FinalNode : public Node {
public:
	FinalNode()
	{
		variable_name = "final";
		node_name = "Final node";
		input_pins = { "Final Output" };
		output_pins = {};
		is_final_node = true;
		title_color = imgui_colors::GREEN;
		init();
	}
};
