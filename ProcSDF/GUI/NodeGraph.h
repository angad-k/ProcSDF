#include <stdio.h>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "GUI/Nodes/Node.h"
#pragma once
class NodeGraph
{
private:
	static NodeGraph* nodeGraph;
public:
	void initialize();
	int allocate_id(Node* p_node);
	void add_node(NodeTypes p_nodeType);
	static NodeGraph* get_singleton() {
		if (!nodeGraph)
		{
			nodeGraph = new NodeGraph();
		}
		return nodeGraph;
	}

	static int get_hash(int x, int y) {
		return (x << 16) + y;
	}

	std::map <int, std::set<int>> adjacency_list;
	std::map <int, Node*> allocated_ids;
	void add_link(int src, int dest);

	void draw();
};