#pragma once
#include <string>
class Node {
public:
	int id;
	bool is_final_node = false;
	virtual void draw() = 0;
	virtual std::string get_string() = 0;
	enum class NodeTypes node_type;
};

enum class NodeTypes {
	FINAL_NODE,
	SPHERE_PRIMITIVE,
};

// TO DO : find a better way to do this
static std::string get_node_type(enum class NodeTypes node_type) {
	switch (node_type)
	{
	case NodeTypes::FINAL_NODE:
		return "Final Node";
		break;
	case NodeTypes::SPHERE_PRIMITIVE:
		return "Sphere Node";
		break;
	}
}

enum class NodeClass {
	PRIMITIVE,
	OPERATION,
	FINAL_OBJECT,
	FINAL
};

class FinalNode : public Node {
private:
	int input_id;
public:
	void draw();
	FinalNode();
	std::string get_string() { return ""; }
};