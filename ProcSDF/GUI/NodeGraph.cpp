#include <set>
#include "GUI/NodeGraph.h"
#pragma once

void NodeGraph::initialize()
{
	// nothing needed for now - we'll add stuff as needed.
	HelloNode* hn = new HelloNode();
	FinalNode* fn = new FinalNode();
	int hn_id = NodeGraph::allocate_id(hn);
	int fn_id = NodeGraph::allocate_id(fn);

	NodeGraph::adjacency_list[hn_id] = std::set<int>();
	NodeGraph::adjacency_list[fn_id] = std::set<int>();

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
	for (auto i : adjacency_list)
	{
		for (int j : i.second) {

			if (j == dest)
			{
				if (!allocated_ids[j]->is_final_node)
				{
					possible = false;
				}
			}
		}
	}
	if (possible)
	{
		NodeGraph::adjacency_list[src].insert(dest);
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
	case NodeTypes::HELLO_NODE:
		new_node = new HelloNode();
		break;
	case NodeTypes::SPHERE_PRIMITIVE:
		new_node = new SphereNode();
		break;
	}
	if (new_node)
	{
		int node_hash = NodeGraph::allocate_id(new_node);
		NodeGraph::adjacency_list[node_hash] = std::set<int>();
	}
}