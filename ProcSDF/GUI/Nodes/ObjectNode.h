#pragma once
#include "GUI/Nodes/Node.h"
#include "GUI/NodeGraph.h"

class ObjectNode : public Node {
public:
	ObjectNode()
	{
		variable_name = "object";
		node_name = "Object node";
		input_pins = { "Input" };
		output_pins = { "Final Output" };
		is_object_node = true;
		title_color = imgui_colors::PURPLE;
		init();
	}
};
