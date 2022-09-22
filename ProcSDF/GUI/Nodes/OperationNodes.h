#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
#include <unordered_map>
class OperationNode : public Node {
public:
	OperationNode()
	{
		title_color = imgui_colors::BLUE;
	}
};
class IntersectionNode : public OperationNode {
public:
	std::string get_string() { return ""; }
	static std::unordered_map<int, bool> allocated_variable_ids;
	IntersectionNode()
	{
		node_name = "Intersection";
		int i = 0;
		while (allocated_variable_ids.find(i) != allocated_variable_ids.end() || allocated_variable_ids[i] == true)
		{
			i++;
		}
		variable_name = "intersection_" + i;
		input_pins = { "Input 1", "Input 2" };
		output_pins = { "Output" };
		is_final_node = false;
		init();
	}
};
