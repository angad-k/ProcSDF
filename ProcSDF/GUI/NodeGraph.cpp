#include "GUI/NodeGraph.h"
#include "GUI/Nodes/PrimitiveNodes.h"
#include "GUI/Nodes/OperationNodes.h"
#include "GUI/Nodes/FinalNode.h"
#include "Utilities/logger.cpp"
#pragma once

void NodeGraph::initialize()
{
	// nothing needed for now - we'll add stuff as needed.
	SphereNode* sn = new SphereNode();
	FinalNode* fn = new FinalNode();
	nodes.push_back(sn);
	nodes.push_back(fn);
}

int NodeGraph::allocate_id(Node* p_node)
{
	for (int i = 0; i < INT_MAX; i++)
	{
		if (allocated_ids.find(i) == allocated_ids.end())
		{
			allocated_ids[i] = p_node;
			return i;
		}
	}

	// TO DO : handle too many allocations
}

void NodeGraph::add_link(int src, int dest)
{
	bool possible = true;
	for (auto link : links)
	{
		if (link.second == dest)
		{
			if (!allocated_ids[link.second]->is_final_node)
			{
				possible = false;
			}
		}
	}
	if (possible)
	{
		links.push_back(std::make_pair(src, dest));
	}
	else
	{
		// TO DO : Toasting mechanism needed here.
	}
}

void NodeGraph::add_node(Node* p_new_node)
{
	if (p_new_node)
	{
		nodes.push_back(p_new_node);
	}
	print_node_graph();
	std::pair<bool, std::vector<int>> pr = NodeGraph::get_topological_sorting();
	logger::log("Cycle" + pr.first);
	logger::log("Topo sorting : \n");
	for (int i : pr.second) {
		logger::log(i + " ");
	}
	logger::log("\n");
}

void NodeGraph::set_adjacency_list() {

	std::map<int, std::set<int>> adjacency_list;
	for (auto link : NodeGraph::links) {
		adjacency_list[allocated_ids[link.first]->id].insert(allocated_ids[link.second]->id);
	}
     
	NodeGraph::adjacency_list = adjacency_list;
}

void NodeGraph::depth_first_search_for_topological_sorting(int src, std::map<int,bool> &visited, std::vector<int>& topological_sorting) {

	visited[src] = true;
	for (int i : NodeGraph::adjacency_list[src]) {
		if (!visited[i]) {
			NodeGraph::depth_first_search_for_topological_sorting(i, visited, topological_sorting);
		}
	}

	topological_sorting.push_back(src);
}

std::pair<bool, std::vector<int>> NodeGraph::get_topological_sorting() {
	
	bool contains_cycle = false;

	std::map<int, bool> visited;
	std::vector<int> topological_sorting;
	for (auto i : NodeGraph::adjacency_list) {
		visited[i.first] = false;
	}

	for (auto i : NodeGraph::adjacency_list) {
		if (!visited[i.first]) {
			NodeGraph::depth_first_search_for_topological_sorting(i.first, visited, topological_sorting);
		}
	}

	std::reverse(topological_sorting.begin(), topological_sorting.end());

	std::map<int, int> node_index_in_topological_sorting;

	for (int i = 0; i < topological_sorting.size(); i++) {
		node_index_in_topological_sorting[topological_sorting[i]] = i;
	}

	for (auto it : NodeGraph::adjacency_list) {
		for (int i : it.second) {
			if (node_index_in_topological_sorting[it.first] > node_index_in_topological_sorting[i]) {
				contains_cycle = true;
				break;
			}
		}
	}

	return std::make_pair(contains_cycle, topological_sorting);
}

void NodeGraph::print_node_graph()
{
	logger::log("Nodes are :\n");
	for (auto it : NodeGraph::allocated_ids)
	{
		std::cout << "id : " << it.first << " | type : " << it.second->node_name << std::endl;
	}

	logger::log("Links are :\n");
	for (auto it : links)
	{
		//fix with log after adding string formatting utility
		std::cout << NodeGraph::allocated_ids[it.first]->node_name << " ( "<< NodeGraph::allocated_ids[it.first]->id << " ) -> " << NodeGraph::allocated_ids[it.second]->node_name << " ( " << NodeGraph::allocated_ids[it.second]->id << " ) " << std::endl;
	}

	NodeGraph::set_adjacency_list();
	auto adjacency_list = NodeGraph::adjacency_list;

	logger::log(" Adjacency list :\n");
	for (auto it : adjacency_list) {
		//fix with log after adding string formatting utility
		std::cout << NodeGraph::allocated_ids[it.first]->node_name <<" ( "<<it.first<<" ) "<< " : ";
		for(int j : it.second) {
			std::cout << NodeGraph::allocated_ids[j]->node_name << " ( " << j << " ) " << " , ";
		}
		logger::log("\n");
	}


}