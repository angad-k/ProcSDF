#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"
#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include "Renderer.h"
#include "GUI/Nodes/Node.h"
class NodeEditor
{
private:
	static NodeEditor* nodeEditor;
	std::vector<int> selected_nodes;
public:
	//void initialize();
	static NodeEditor* get_singleton() {
		if (!nodeEditor)
		{
			nodeEditor = new NodeEditor();
		}
		return nodeEditor;
	}

	std::vector<int> get_selected_nodes()
	{
		return selected_nodes;
	}

	void draw();
};