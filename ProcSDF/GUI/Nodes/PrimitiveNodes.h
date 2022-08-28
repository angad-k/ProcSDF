#pragma once
#include "GUI/Nodes/Node.h"
#include <string>
class SphereNode : public Node {
private:
	int output_id;

public:
	void draw();
	SphereNode();
	std::string get_string() { return ""; }
};