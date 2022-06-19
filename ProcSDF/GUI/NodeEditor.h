#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"
#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include "Renderer.h"
#include "Node.h"
class NodeEditor
{
private:
	static NodeEditor* nodeEditor;
	std::map <int, Node*> allocated_ids;
	std::vector <Node*> nodes;
	std::vector<std::pair<int, int>> links;
	void add_link(int src, int dest);
public:
	void initialize();
	int allocate_id(Node* p_node);
	void add_node(NodeTypes p_nodeType);
	static NodeEditor* get_singleton() {
		if (!nodeEditor)
		{
			nodeEditor = new NodeEditor();
		}
		return nodeEditor;
	}

	void draw();
};