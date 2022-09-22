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
	Node* get_node(int id);
	void add_node(Node* p_new_node);
	void set_adjacency_list();
	std::pair<bool, std::vector<int>> get_topological_sorting();
	void depth_first_search_for_topological_sorting(int src, std::map<int,bool> &visited, std::vector<int>& topological_sorting);

	Node* get_source_node(int dest_id);
	int get_source_id(int dest_id);

	static NodeGraph* get_singleton() {
		if (!nodeGraph)
		{
			nodeGraph = new NodeGraph();
		}
		return nodeGraph;
	}
	std::map<int, std::set<int>> adjacency_list;
	std::vector <Node*> nodes;
	std::vector<std::pair<int, int>> links;
	std::map <int, Node*> allocated_ids;
	void add_link(int src, int dest);
	void print_node_graph();
};