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
	static NodeGraph* s_nodeGraph;
	void depthFirstSearchForTopologicalSorting(int p_src, std::vector<int>& p_topologicalSorting,
		std::vector<TransformNode*>& p_operationOrdering,
		Node* p_previousNonTransformNode = NULL);
	std::map <int, Node*> m_allocatedIDs;
	std::map <std::string, std::string> m_customNodeFilePathToContent;
	bool m_dirty = false;
public:
	void initialize();
	int allocateID(Node* p_node);
	void deallocateID(int p_id);
	Node* getNode(int p_id);
	void deleteNode(int p_id);
	void addNode(Node* p_newNode);
	void setAdjacencyList();
	std::vector<int> getTopologicalSorting();
	Node* getSourceNode(int p_destID);
	int getSourceId(int p_destID);
	void addLink(int p_src, int p_dest);
	void removeLinkWithEndpoint(int p_endpoint);
	void removeLinkWithEndpoints(std::vector<int> p_endpoints);
	void printNodeGraph();
	void recompileNodeGraph();
	bool isIterable(int p_count, int p_inputPinSize);
	void clear();

	bool m_errorInCompilation;
	std::string m_compilationError;
	FinalNode* m_finalNode;
	// vector of pairs correspond to the node id and output ids
	std::map<int, std::vector<int>> m_adjacencyList;
	std::vector <Node*> m_nodes;
	std::vector<std::pair<int, int>> m_links;
	std::map <int, std::set<int>> m_reachableObjects;

	void setCustomNodeFileContents(std::string p_fileName, std::string p_fileContents)
	{
		m_customNodeFilePathToContent[p_fileName] = p_fileContents;
	}

	std::vector<std::string> getCustomNodeFileContents()
	{
		std::vector<std::string> l_customNodeFileContents;
		for (auto path_content : m_customNodeFilePathToContent)
		{
			l_customNodeFileContents.push_back(path_content.second);
		}
		return l_customNodeFileContents;
	}
	
	std::string getCompilationError()
	{
		return m_compilationError;
	}
	void setCompilationError(std::string p_compilation_error)
	{
		m_compilationError = p_compilation_error;
		m_errorInCompilation = true;
	}
	void clearCompilationError()
	{
		m_compilationError.clear();
		m_errorInCompilation = false;
	}
	bool checkCompilationError()
	{
		return m_errorInCompilation;
	}
	bool isDirty()
	{
		return m_dirty;
	}
	void informModification()
	{
		m_dirty = true;
	}

	static NodeGraph* getSingleton() {
		if (!s_nodeGraph)
		{
			s_nodeGraph = new NodeGraph();
		}
		return s_nodeGraph;
	}
};