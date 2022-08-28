#include <stdio.h>
#include <iostream>
#include <map>
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
	std::map<int, std::set<int>> get_adjacency_list();

	static NodeGraph* get_singleton() {
		if (!nodeGraph)
		{
			nodeGraph = new NodeGraph();
		}
		return nodeGraph;
	}
	std::vector <Node*> nodes;
	std::vector<std::pair<int, int>> links;
	std::map <int, Node*> allocated_ids;
	void add_link(int src, int dest);
	void print_node_graph();
};