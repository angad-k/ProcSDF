#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
#include <unordered_map>
class PrimitiveNode : public Node {
public:
	PrimitiveNode()
	{
		title_color = imgui_colors::RED;
	}
};
class SphereNode : public PrimitiveNode {
public:
	std::string get_string() { return ""; }
	static std::unordered_map<int, bool> allocated_variable_ids;
	SphereNode()
	{
		node_name = "Sphere";
		int i = 0;
		while (allocated_variable_ids.find(i) != allocated_variable_ids.end() || allocated_variable_ids[i] == true)
		{
			i++;
		}
		variable_name = "sphere_" + i;
		input_pins = {};
		output_pins = { "Output" };
		is_final_node = false;
		input_float3_labels = { "Centre Position" };
		input_float_labels = { "Radius" };
		init();
	}
};
