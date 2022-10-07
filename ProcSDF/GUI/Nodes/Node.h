#pragma once
#include <string>
#include <tuple>
#include <vector>
#include "Constants/constant.h"
class Node {
public:
	int id;
	bool is_final_node = false;
	bool is_object_node = false;
	void draw();
	std::string get_string();

	std::vector <std::string> input_pins;
	std::vector <int> input_ids;
	std::vector <std::string> output_pins;
	std::vector <int> output_ids;
	std::tuple <int, int, int> coordinate_offset;

	std::vector <std::string> input_float3_labels;
	std::vector <std::vector<float>> input_float3;
	std::vector <std::string> input_float_labels;
	std::vector <float> input_floats;

	std::string node_name;
	std::string variable_name;
	int title_color = imgui_colors::BLUE;

	std::string get_variable_name()
	{
		return variable_name;
	}
	void init();
};
