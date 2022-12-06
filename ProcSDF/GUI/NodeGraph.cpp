#include <algorithm>
#include <ranges>
#include "GUI/NodeGraph.h"
#include "GUI/Nodes/PrimitiveNodes.h"
#include "GUI/Nodes/OperationNodes.h"
#include "GUI/Nodes/FinalNode.h"
#include "GUI/Nodes/ObjectNode.h"
#include "Common/logger.h"
#include "Rendering/ShaderGenerator.h"
#pragma once

void NodeGraph::initialize()
{
	m_errorInCompilation = false;
	SphereNode* l_sn = new SphereNode();
	l_sn->m_inputFloats[0] = 2.0;
	FinalNode* l_fn = new FinalNode();
	ObjectNode* l_on = new ObjectNode();
	NodeGraph::addLink(l_sn->m_outputIDs[0], l_on->m_inputIDs[0]);
	NodeGraph::addLink(l_on->m_outputIDs[0], l_fn->m_inputIDs[0]);
	m_finalNode = l_fn;
	m_nodes.push_back(l_sn);
	m_nodes.push_back(l_fn);
	m_nodes.push_back(l_on);
}

void NodeGraph::clear()
{
	for (int i = 0; i < m_nodes.size(); i++) {
		delete(m_nodes[i]);
	}
	delete(m_finalNode);
	delete(s_nodeGraph);
	s_nodeGraph = NULL;
}

int NodeGraph::allocateID(Node* p_node)
{
	for (int i = 0; i < INT_MAX; i++)
	{
		if (m_allocatedIDs.find(i) == m_allocatedIDs.end())
		{
			m_allocatedIDs[i] = p_node;
			return i;
		}
	}

	// TO DO : handle too many allocations
}

void NodeGraph::deallocateID(int p_id)
{
	if (m_allocatedIDs.find(p_id) != m_allocatedIDs.end())
	{
		m_allocatedIDs.erase(p_id);
	}
}

Node* NodeGraph::getNode(int p_id)
{
	return m_allocatedIDs[p_id];
}

void NodeGraph::addLink(int p_src, int p_dest)
{
	bool l_possible = true;
	for (auto link : m_links)
	{
		if (link.second == p_dest)
		{
			if (!m_allocatedIDs[link.second]->m_isFinalNode)
			{
				ERR("All input pins(except for final) can have only one input.");
				l_possible = false;
				break;
			}
		}
	}
	if ((m_allocatedIDs[p_src]->m_isObjectNode && !m_allocatedIDs[p_dest]->m_isFinalNode)
		|| ((!m_allocatedIDs[p_src]->m_isObjectNode) && m_allocatedIDs[p_dest]->m_isFinalNode))
	{
		ERR("|| Object Node --> Final Node || is the only acceptable link here.");
		l_possible = false;
	}
	if (l_possible)
	{
		m_links.push_back(std::make_pair(p_src, p_dest));
		m_dirty = true;
	}
}

void NodeGraph::removeLinkWithEndpoint(int p_endpoint)
{
	std::vector<std::pair<int, int>>l_filteredLinks;
	for (std::pair<int, int> link : m_links)
	{
		if (link.first != p_endpoint && link.second != p_endpoint)
		{
			l_filteredLinks.push_back(link);
		}
	}
	m_links = l_filteredLinks;
}

void NodeGraph::removeLinkWithEndpoints(std::vector<int> p_endpoints)
{
	std::vector<std::pair<int, int>>m_filteredLinks;
	for (std::pair<int, int> link : m_links)
	{
		if (std::find(p_endpoints.begin(), p_endpoints.end(), link.first) == p_endpoints.end() &&
			std::find(p_endpoints.begin(), p_endpoints.end(), link.second) == p_endpoints.end())
		{
			m_filteredLinks.push_back(link);
		}
	}
	m_links = m_filteredLinks;
}

void NodeGraph::addNode(Node* p_new_node)
{
	if (p_new_node)
	{
		m_nodes.push_back(p_new_node);
		m_dirty = true;
	}
}

void NodeGraph::deleteNode(int p_id)
{
	Node* node = m_allocatedIDs[p_id];
	if (node->m_isFinalNode)
	{
		ERR("Cannot delete final node.")
			return;
	}
	m_nodes.erase(std::find(m_nodes.begin(), m_nodes.end(), node));
	delete(node);
	m_dirty = true;
}

void NodeGraph::setAdjacencyList() {

	std::map<int, std::set<int>> l_adjacencyListSet;
	std::map<int, std::vector<int>> l_adjacencyList;
	for (auto link : NodeGraph::m_links) {
		l_adjacencyListSet[NodeGraph::getNode(link.first)->m_ID].insert(NodeGraph::getNode(link.second)->m_ID);
	}

	for (auto it : l_adjacencyListSet) {
		l_adjacencyList[it.first] = std::vector<int>(it.second.begin(), it.second.end());
	}

	NodeGraph::m_adjacencyList = l_adjacencyList;
}

void NodeGraph::depthFirstSearchForTopologicalSorting(int p_src, 
	std::vector<int>& p_topologicalSorting,
	std::vector<TransformNode*>& p_operationOrdering,
	Node* p_previousNonTransformNode) {

	Node* l_srcNode = NodeGraph::getNode(p_src);
	std::set<int> l_childObjectNodes, l_objectNodesSubset;
	std::vector<int> l_mergeOutput;
	int l_index = 0;

	l_srcNode->m_previousNonTransformNode.push_back(p_previousNonTransformNode);

	if (!l_srcNode->m_isTransformNode) {
		p_previousNonTransformNode = l_srcNode;
		l_srcNode->m_operationOrdering[l_srcNode->m_visitCount] = p_operationOrdering;
		p_operationOrdering.clear();
	}
	else {
		p_operationOrdering.push_back((TransformNode*)l_srcNode);
	}

	// TODO : make this efficient by merging all the child object corresponding to each child node and then iterate.

	for (int i : NodeGraph::m_adjacencyList[p_src]) {

		Node* l_itrNode = NodeGraph::getNode(i);
		if (NodeGraph::isIterable(l_itrNode->m_visitCount, l_itrNode->m_inputPins.size())) {
			NodeGraph::depthFirstSearchForTopologicalSorting(i, p_topologicalSorting, p_operationOrdering, p_previousNonTransformNode);
		}

		Node* l_childNode = NodeGraph::getNode(i);

		l_objectNodesSubset = NodeGraph::m_reachableObjects[i];
		std::merge(l_childObjectNodes.begin(), l_childObjectNodes.end(), l_objectNodesSubset.begin(), l_objectNodesSubset.end(), std::back_inserter(l_mergeOutput));
		l_childObjectNodes = std::set<int>(l_mergeOutput.begin(), l_mergeOutput.end());
		l_index++;
	}

	if (l_srcNode->m_isObjectNode) {
		l_childObjectNodes.insert(p_src);
	}
	
	if (l_srcNode->m_visitCount == 0) {
		p_topologicalSorting.push_back(p_src);
	}
	NodeGraph::m_reachableObjects[l_srcNode->m_ID] = l_childObjectNodes;
	l_srcNode->m_visitCount++;
}

bool NodeGraph::isIterable(int visit_count, int input_pin_size) {
	if (visit_count < input_pin_size) {
		return true;
	}
	else {
		return false;
	}
}

Node* NodeGraph::getSourceNode(int dest_id)
{
	int src_id = getSourceId(dest_id);
	if (src_id == -1)
	{
		return nullptr;
	}
	return m_allocatedIDs[src_id];
}

int NodeGraph::getSourceId(int dest_id)
{
	for (auto it : m_links)
	{
		if (it.second == dest_id)
		{
			return it.first;
		}
	}
	return -1;
}

std::vector<int> NodeGraph::getTopologicalSorting() {

	std::vector<int> l_topologicalSorting;
	std::vector<TransformNode*> l_operationOrder;

	for (Node* i : NodeGraph::m_nodes) {
		i->m_visitCount = 0;
		i->m_previousNonTransformNode.clear();
		i->m_operationOrdering.clear();
	}

	for (auto i : NodeGraph::m_adjacencyList) {
		if (NodeGraph::getNode(i.first)->m_inputPins.size() == 0) {
			NodeGraph::depthFirstSearchForTopologicalSorting(i.first, l_topologicalSorting, l_operationOrder, NULL);
		}
	}

	std::reverse(l_topologicalSorting.begin(), l_topologicalSorting.end());

	std::map<int, int> l_nodeIndexInTopologicalSorting;

	for (int i = 0; i < l_topologicalSorting.size(); i++) {
		l_nodeIndexInTopologicalSorting[l_topologicalSorting[i]] = i;
	}

	bool l_containsCycle = false;
	for (auto it : NodeGraph::m_adjacencyList) {
		for (int i : it.second) {
			if (l_nodeIndexInTopologicalSorting[it.first] > l_nodeIndexInTopologicalSorting[i]) {
				l_containsCycle = true;
				break;
			}
		}
	}

	if (l_containsCycle)
	{
		std::string l_compilationError = "Node Graph contains cycle";
		setCompilationError(l_compilationError);
		ERR(l_compilationError);
	}

	return l_topologicalSorting;
}

void NodeGraph::printNodeGraph()
{
	logger::log("Nodes are :\n");
	for (auto it : NodeGraph::m_allocatedIDs)
	{
		std::cout << "id : " << it.first << " | type : " << it.second->m_nodeName << std::endl;
	}

	logger::log("Links are :\n");
	for (auto it : m_links)
	{
		//fix with log after adding string formatting utility
		std::cout << NodeGraph::m_allocatedIDs[it.first]->m_nodeName << " ( " << NodeGraph::m_allocatedIDs[it.first]->m_ID << " ) -> " << NodeGraph::m_allocatedIDs[it.second]->m_nodeName << " ( " << NodeGraph::m_allocatedIDs[it.second]->m_ID << " ) " << std::endl;
	}

	NodeGraph::setAdjacencyList();
	auto l_adjacencyList = NodeGraph::m_adjacencyList;

	logger::log(" Adjacency list :\n");
	for (auto it : l_adjacencyList) {
		//fix with log after adding string formatting utility
		std::cout << NodeGraph::m_allocatedIDs[it.first]->m_nodeName << " ( " << it.first << " ) " << " : ";
		for (int j : it.second) {
			std::cout << NodeGraph::m_allocatedIDs[j]->m_nodeName << " ( " << j << " ) " << " , ";
		}
		logger::log("\n");
	}


}

void NodeGraph::recompileNodeGraph()
{
	m_dirty = false;
	NodeGraph::clearCompilationError();
	printNodeGraph();
	std::vector<int> l_topologicallySortedNodes = NodeGraph::getTopologicalSorting();
	if (checkCompilationError())
	{
		return;
	}
	logger::log("Topo sorting : \n");
	for (int i : l_topologicallySortedNodes) {
		Node* l_topoNode = m_allocatedIDs[i];
		std::string l_topoString = l_topoNode->m_getString();
		if (checkCompilationError())
		{
			return;
		}
		logger::log(std::to_string(i) + " -> ");
		logger::log(l_topoString);
		logger::log("\n");
	}
	logger::log("\n");

	ShaderGenerator::getSingleton()->generateAndSetShader();
}
