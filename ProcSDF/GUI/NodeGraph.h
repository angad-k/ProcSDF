#include <stdio.h>
#include <iostream>
#include <map>
#include <map>
#include <set>
#include <vector>
#include <tuple>

#include "GUI/Nodes/Node.h"
#include "GUI/Nodes/FinalNode.h"
#pragma once
class NodeGraph
{
private:
	static NodeGraph* nodeGraph;
	void depth_first_search_for_topological_sorting(int src, std::map<int, bool>& visited, 
		std::vector<int>& topological_sorting, Node* previous_non_transform_node, 
		std::vector<std::tuple<int, std::vector<int>>>& operation_ordering);
	bool dirty = false;
public:
	void initialize();
	int allocate_id(Node* p_node);
	void deallocate_id(int id);
	Node* get_node(int id);
	void delete_node(int p_id);
	void add_node(Node* p_new_node);
	void set_adjacency_list();
	std::vector<int> get_topological_sorting();
	Node* get_source_node(int dest_id);
	int get_source_id(int dest_id);
	void add_link(int src, int dest);
	void remove_link_with_endpoint(int p_endpoint);
	void remove_link_with_endpoints(std::vector<int> p_endpoints);
	void print_node_graph();
	void recompile_node_graph();
	bool is_iterable(int count, bool is_operation_node);

	bool error_in_compilation;
	std::string compilation_error;
	FinalNode* final_node;
	// vector of pairs correspond to the node id and output ids
	std::map<int, std::vector<int>> adjacency_list;
	std::map<int, std::vector<int>> adjacency_list_with_output_ids;
	std::vector <Node*> nodes;
	std::vector<std::pair<int, int>> links;
	std::map <int, std::set<int>> reachable_objects;
	
	std::string get_compilation_error()
	{
		return compilation_error;
	}
	void set_compilation_error(std::string p_compilation_error)
	{
		compilation_error = p_compilation_error;
		error_in_compilation = true;
	}
	void clear_compilation_error()
	{
		compilation_error.clear();
		error_in_compilation = false;
	}
	bool check_compilation_error()
	{
		return error_in_compilation;
	}
	bool is_dirty()
	{
		return dirty;
	}
	void inform_modification()
	{
		dirty = true;
	}

	static NodeGraph* get_singleton() {
		if (!nodeGraph)
		{
			nodeGraph = new NodeGraph();
		}
		return nodeGraph;
	}
};