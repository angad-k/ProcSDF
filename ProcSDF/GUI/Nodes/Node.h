#pragma once
#include <string>
#include <vector>
#include "Constants/constant.h"
class Node {
public:
	int id;
	bool is_final_node = false;
	void draw();
	virtual std::string get_string() = 0;

	std::vector <std::string> input_pins;
	std::vector <int> input_ids;
	std::vector <std::string> output_pins;
	std::vector <int> output_ids;

	std::vector <std::string> input_float3_labels;
	std::vector <std::vector<float>> input_float3;
	std::vector <std::string> input_float_labels;
	std::vector <float> input_floats;

	std::string node_name;
	int title_color = imgui_colors::BLUE;
	void init();
};