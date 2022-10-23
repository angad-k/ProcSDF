#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <map>
#include "Constants/constant.h"
class Node {
public:
	int id;
	bool is_final_node = false;
	bool is_object_node = false;
	bool is_tranform_node = false;
	void draw();
	std::string get_string();

	std::vector <std::string> input_pins;
	std::vector <int> input_ids;
	std::vector <std::string> output_pins;
	std::vector <int> output_ids;
	std::map <int, std::tuple <float, float, float>> coordinate_offset_for_objects;
	std::map <int, std::vector<std::tuple<int, float>>> rotation_offset_for_objects;

	std::vector <std::string> input_float3_labels;
	std::vector <std::vector<float>> input_float3;
	std::vector <std::string> input_float_labels;
	std::vector <float> input_floats;

	std::string node_name;
	std::string variable_name;
	Node* previous_non_transform_node;
	int title_color = imgui_colors::BLUE;

	std::string get_variable_name()
	{
		return variable_name;
	}
	void init();

	~Node();
};
