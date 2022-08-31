#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
class IntersectionNode : public Node {
private:
	int output_id;
	int input_id_1;
	int input_id_2;

public:
	void draw();
	IntersectionNode();
	std::string get_string() { return ""; }
};