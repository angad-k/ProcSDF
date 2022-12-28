#include <stdio.h>
#include <iostream>
#include <map>
#include <map>
#include <set>
#include <vector>
#include <tuple>
#pragma once
#include "GUI/Nodes/Node.h"
#include "GUI/Nodes/FinalNode.h"
class Material;
class NodeGraph
{
private:
	static NodeGraph* s_nodeGraph;
	void depthFirstSearchForTopologicalSorting(int p_src, std::vector<int>& p_topologicalSorting,
		std::vector<TransformNode*>& p_operationOrdering,
		Node* p_previousNonTransformNode = NULL);
	std::map <int, Node*> m_allocatedIDs;
	std::map <std::string, std::string> m_customNodeNameToContent;
	std::map <std::string, std::string> m_customNodeNameToFilePath;
	std::map <std::string, std::string> m_customMaterialNameToContent;
	bool m_dirty = false;
	std::vector<Material*>m_materials;
	std::map <int, Material*> m_allocatedMaterialIDs;
public:
	void initialize();
	
	int allocateID(Node* p_node);
	int allocateMaterialID(Material* p_material);
	void deallocateMaterialID(int p_ID);
	void deleteMaterialAt(int p_pos);
	void fixMaterials();
	bool canDeleteMaterial()
	{
		return m_materials.size() > 1;
	}
	void setID(Node* p_node, int p_ID);
	void setMaterialID(Material* p_material, int p_ID);
	void deallocateID(int p_ID);
	Node* getNode(int p_ID);
	void deleteNode(int p_ID);
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

	void setCustomNodeFileContents(std::string p_nodeName, std::string p_fileContents)
	{
		m_customNodeNameToContent[p_nodeName] = p_fileContents;
	}

	void setCustomNodeFilePath(std::string p_nodeName, std::string p_filePath)
	{
		m_customNodeNameToFilePath[p_nodeName] = p_filePath;
	}

	std::string getCustomNodeFilePath(std::string p_nodeName)
	{
		if (m_customNodeNameToFilePath.find(p_nodeName) == m_customNodeNameToFilePath.end())
		{
			return "";
		}

		return m_customNodeNameToFilePath[p_nodeName];
	}

	std::string getCustomNodeFileContentsfromNodeName(std::string p_nodeName)
	{
		return m_customNodeNameToContent[p_nodeName];
	}

	std::vector<std::string> getCustomNodeFileContents()
	{
		std::vector<std::string> l_customNodeFileContents;
		for (auto path_content : m_customNodeNameToContent)
		{
			l_customNodeFileContents.push_back(path_content.second);
		}
		return l_customNodeFileContents;
	}

	std::vector<std::string> getCustomNodeNames()
	{
		std::vector<std::string> l_customNodeNames;
		for (auto path_content : m_customNodeNameToContent)
		{
			l_customNodeNames.push_back(path_content.first);
		}
		return l_customNodeNames;
	}

	void setCustomMaterialFileContents(std::string p_materialName, std::string p_fileContents)
	{
		m_customMaterialNameToContent[p_materialName] = p_fileContents;
	}
	std::string getCustomMaterialFileContentsfromMaterialName(std::string p_materialName)
	{
		return m_customMaterialNameToContent[p_materialName];
	}

	std::vector<std::string> getCustomMaterialFileContents()
	{
		std::vector<std::string> l_customMaterialFileContents;
		for (auto path_content : m_customMaterialNameToContent)
		{
			l_customMaterialFileContents.push_back(path_content.second);
		}
		return l_customMaterialFileContents;
	}

	std::vector<std::string> getCustomMaterialNames()
	{
		std::vector<std::string> l_customMaterialNames;
		for (auto path_content : m_customMaterialNameToContent)
		{
			l_customMaterialNames.push_back(path_content.first);
		}
		return l_customMaterialNames;
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

	std::vector<Material*> getMaterials();

	void addDiffuse();
	void addMetal();
	void addDielectric();
	void addLight();
	void addMaterial(Material* p_material);

	Material* getMaterialFromMaterialID(int p_id);

	static NodeGraph* getSingleton() {
		if (!s_nodeGraph)
		{
			s_nodeGraph = new NodeGraph();
		}
		return s_nodeGraph;
	}
};