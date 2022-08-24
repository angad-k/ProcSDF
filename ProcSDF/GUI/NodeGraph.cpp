#include "GUI/NodeGraph.h"
#pragma once

void NodeGraph::initialize()
{
	// nothing needed for now - we'll add stuff as needed.
	HelloNode* hn = new HelloNode();
	FinalNode* fn = new FinalNode();
	nodes.push_back(hn);
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
	case NodeTypes::HELLO_NODE:
		new_node = new HelloNode();
		break;
	case NodeTypes::SPHERE_PRIMITIVE:
		new_node = new SphereNode();
		break;
	}
	if (new_node)
	{
		nodes.push_back(new_node);
	}
}