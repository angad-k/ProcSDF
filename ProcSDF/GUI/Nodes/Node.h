#pragma once
class Node {
public:
	int id;
	bool is_final_node = false;
	virtual void draw() = 0;
};

enum class NodeTypes {
	HELLO_NODE,
	FINAL_NODE,
	SPHERE_PRIMITIVE,
};

class HelloNode : public Node{
private:
	int input_id_1;
	int input_id_2;
	int output_id_1;
	int output_id_2;
public:
	void draw();
	HelloNode();
};

class SphereNode : public Node {
private:
	int output_id;

public:
	void draw();
	SphereNode();
};

class FinalNode : public Node{
private:
	int input_id;
public:
	void draw();
	FinalNode();
};