#include <algorithm>
#include <ranges>
#include "GUI/NodeGraph.h"
#include "GUI/Nodes/PrimitiveNodes.h"
#include "GUI/Nodes/OperationNodes.h"
#include "GUI/Nodes/FinalNode.h"
#include "GUI/Nodes/ObjectNode.h"
#include "Utilities/logger.h"
#include "Rendering/ShaderGenerator.h"
#pragma once

void NodeGraph::initialize()
{
	error_in_compilation = false;
	SphereNode* sn = new SphereNode();
	sn->input_floats[0] = 2.0;
	FinalNode* fn = new FinalNode();
	ObjectNode* on = new ObjectNode();
	NodeGraph::add_link(sn->output_ids[0], on->input_ids[0]);
	NodeGraph::add_link(on->output_ids[0], fn->input_ids[0]);
	final_node = fn;
	nodes.push_back(sn);
	nodes.push_back(fn);
	nodes.push_back(on);
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

void NodeGraph::deallocate_id(int id)
{
	if (allocated_ids.find(id) != allocated_ids.end())
	{
		allocated_ids.erase(id);
	}
}

Node* NodeGraph::get_node(int id)
{
	return allocated_ids[id];
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
				ERR("All input pins(except for final) can have only one input.");
				possible = false;
				break;
			}
		}
	}
	if ((allocated_ids[src]->is_object_node && !allocated_ids[dest]->is_final_node)
		|| ((!allocated_ids[src]->is_object_node) && allocated_ids[dest]->is_final_node))
	{
		ERR("|| Object Node --> Final Node || is the only acceptable link here.");
		possible = false;
	}
	if (possible)
	{
		links.push_back(std::make_pair(src, dest));
		dirty = true;
	}
}

void NodeGraph::remove_link_with_endpoint(int p_endpoint)
{
	std::vector<std::pair<int, int>>filtered_links;
	for (std::pair<int, int> link : links)
	{
		if (link.first != p_endpoint && link.second != p_endpoint)
		{
			filtered_links.push_back(link);
		}
	}
	links = filtered_links;
}

void NodeGraph::remove_link_with_endpoints(std::vector<int> p_endpoints)
{
	std::vector<std::pair<int, int>>filtered_links;
	for (std::pair<int, int> link : links)
	{
		if (std::find(p_endpoints.begin(), p_endpoints.end(), link.first) == p_endpoints.end() &&
			std::find(p_endpoints.begin(), p_endpoints.end(), link.second) == p_endpoints.end())
		{
			filtered_links.push_back(link);
		}
	}
	links = filtered_links;
}

void NodeGraph::add_node(Node* p_new_node)
{
	if (p_new_node)
	{
		nodes.push_back(p_new_node);
		dirty = true;
	}
}

void NodeGraph::delete_node(int p_id)
{
	Node* node = allocated_ids[p_id];
	if (node->is_final_node)
	{
		ERR("Cannot delete final node.")
			return;
	}
	nodes.erase(std::find(nodes.begin(), nodes.end(), node));
	delete(node);
	dirty = true;
}

void NodeGraph::set_adjacency_list() {

	std::map<int, std::set<int>> adjacency_list_set;
	std::map<int, std::vector<int>> adjacency_list;
	for (auto link : NodeGraph::links) {
		adjacency_list_set[NodeGraph::get_node(link.first)->id].insert(NodeGraph::get_node(link.second)->id);
	}

	for (auto it : adjacency_list_set) {
		adjacency_list[it.first] = std::vector<int>(it.second.begin(), it.second.end());
	}

	NodeGraph::adjacency_list = adjacency_list;
}

void NodeGraph::depth_first_search_for_topological_sorting(int src, 
	std::vector<int>& topological_sorting,
	std::vector<TransformNode*>& operation_ordering,
	Node* previous_non_transform_node) {

	Node* src_node = NodeGraph::get_node(src);
	std::set<int> child_object_nodes, object_nodes_subset;
	std::vector<int> merge_output;
	int index = 0;

	src_node->previous_non_transform_node.push_back(previous_non_transform_node);

	if (!src_node->is_transform_node) {
		previous_non_transform_node = src_node;
		src_node->operation_ordering[src_node->visit_count] = operation_ordering;
		operation_ordering.clear();
	}
	else {
		operation_ordering.push_back((TransformNode*)src_node);
	}

	// TODO : make this efficient by merging all the child object corresponding to each child node and then iterate.

	for (int i : NodeGraph::adjacency_list[src]) {

		Node* itr_node = NodeGraph::get_node(i);
		if (NodeGraph::is_iterable(itr_node->visit_count, itr_node->input_pins.size())) {
			NodeGraph::depth_first_search_for_topological_sorting(i, topological_sorting, operation_ordering, previous_non_transform_node);
		}

		Node* child_node = NodeGraph::get_node(i);

		object_nodes_subset = NodeGraph::reachable_objects[i];
		std::merge(child_object_nodes.begin(), child_object_nodes.end(), object_nodes_subset.begin(), object_nodes_subset.end(), std::back_inserter(merge_output));
		child_object_nodes = std::set<int>(merge_output.begin(), merge_output.end());
		index++;
	}

	if (src_node->is_object_node) {
		child_object_nodes.insert(src);
	}
	
	if (src_node->visit_count == 0) {
		topological_sorting.push_back(src);
	}
	NodeGraph::reachable_objects[src_node->id] = child_object_nodes;
	src_node->visit_count++;
}

bool NodeGraph::is_iterable(int visit_count, int input_pin_size) {
	if (visit_count < input_pin_size) {
		return true;
	}
	else {
		return false;
	}
}

Node* NodeGraph::get_source_node(int dest_id)
{
	int src_id = get_source_id(dest_id);
	if (src_id == -1)
	{
		return nullptr;
	}
	return allocated_ids[src_id];
}

int NodeGraph::get_source_id(int dest_id)
{
	for (auto it : links)
	{
		if (it.second == dest_id)
		{
			return it.first;
		}
	}
	return -1;
}

std::vector<int> NodeGraph::get_topological_sorting() {

	std::vector<int> topological_sorting;
	std::vector<TransformNode*> operation_order;

	for (Node* i : NodeGraph::nodes) {
		i->visit_count = 0;
		i->previous_non_transform_node.clear();
		i->operation_ordering.clear();
	}

	for (auto i : NodeGraph::adjacency_list) {
		if (NodeGraph::get_node(i.first)->input_pins.size() == 0) {
			NodeGraph::depth_first_search_for_topological_sorting(i.first, topological_sorting, operation_order, NULL);
		}
	}

	std::reverse(topological_sorting.begin(), topological_sorting.end());

	std::map<int, int> node_index_in_topological_sorting;

	for (int i = 0; i < topological_sorting.size(); i++) {
		node_index_in_topological_sorting[topological_sorting[i]] = i;
	}

	bool contains_cycle = false;
	for (auto it : NodeGraph::adjacency_list) {
		for (int i : it.second) {
			if (node_index_in_topological_sorting[it.first] > node_index_in_topological_sorting[i]) {
				contains_cycle = true;
				break;
			}
		}
	}

	if (contains_cycle)
	{
		std::string compilation_error = "Node Graph contains cycle";
		set_compilation_error(compilation_error);
		ERR(compilation_error);
	}

	return topological_sorting;
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
		std::cout << NodeGraph::allocated_ids[it.first]->node_name << " ( " << NodeGraph::allocated_ids[it.first]->id << " ) -> " << NodeGraph::allocated_ids[it.second]->node_name << " ( " << NodeGraph::allocated_ids[it.second]->id << " ) " << std::endl;
	}

	NodeGraph::set_adjacency_list();
	auto adjacency_list = NodeGraph::adjacency_list;

	logger::log(" Adjacency list :\n");
	for (auto it : adjacency_list) {
		//fix with log after adding string formatting utility
		std::cout << NodeGraph::allocated_ids[it.first]->node_name << " ( " << it.first << " ) " << " : ";
		for (int j : it.second) {
			std::cout << NodeGraph::allocated_ids[j]->node_name << " ( " << j << " ) " << " , ";
		}
		logger::log("\n");
	}


}

void NodeGraph::recompile_node_graph()
{
	dirty = false;
	NodeGraph::clear_compilation_error();
	print_node_graph();
	std::vector<int> topologically_sorted_nodes = NodeGraph::get_topological_sorting();
	if (check_compilation_error())
	{
		return;
	}
	logger::log("Topo sorting : \n");
	for (int i : topologically_sorted_nodes) {
		Node* topo_node = allocated_ids[i];
		std::string topo_string = topo_node->get_string();
		if (check_compilation_error())
		{
			return;
		}
		logger::log(std::to_string(i) + " -> ");
		logger::log(topo_string);
		logger::log("\n");
	}
	logger::log("\n");

	ShaderGenerator::get_singleton()->generate_and_set_shader();
}
