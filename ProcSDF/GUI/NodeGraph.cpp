#include "GUI/NodeGraph.h"
#include "GUI/Nodes/PrimitiveNodes.h"
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

void NodeGraph::add_node(NodeTypes p_nodeType)
{
	Node* new_node = NULL;
	switch (p_nodeType)
	{
	case NodeTypes::SPHERE_PRIMITIVE:
		new_node = new SphereNode();
		break;
	}
	if (new_node)
	{
		nodes.push_back(new_node);
	}
	print_node_graph();
}

std::map<int, std::set<int>> NodeGraph::get_adjacency_list() {

	std::map<int, std::set<int>> adjacency_list;
	for (auto link : NodeGraph::links) {
		adjacency_list[link.first].insert(link.second);
	}

	return adjacency_list;
}

void NodeGraph::print_node_graph()
{
	std::cout << "Nodes are :\n";
	for (auto it : allocated_ids)
	{
		std::cout << "id : " << it.first << " | type : " << get_node_type(it.second->node_type) << std::endl;
	}

	std::cout << "Links are :\n";
	for (auto it : links)
	{
		std::cout << get_node_type(NodeGraph::allocated_ids[it.first]->node_type) << " ( "<<it.first << " ) -> " << get_node_type(NodeGraph::allocated_ids[it.second]->node_type) << " ( " << it.second << " ) " << std::endl;
	}

	auto adjacency_list = get_adjacency_list();

	std::cout << " Adjacency list :\n";
	for (auto it : adjacency_list) {
		std::cout << get_node_type(NodeGraph::allocated_ids[it.first]->node_type) << " : ";
		for(int j : it.second) {
			std::cout << get_node_type(NodeGraph::allocated_ids[j]->node_type) << " , ";
		}
		std::cout << "\n";
	}


}