#pragma once
#include <string>
#include <vector>
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
	std::string node_name;
	void init();
};

class FinalNode : public Node {
public:
	std::string get_string() { return ""; }
	FinalNode()
	{
		node_name = "Final node";
		input_pins = { "Final Output" };
		output_pins = {};
		is_final_node = true;
		init();
	}
};