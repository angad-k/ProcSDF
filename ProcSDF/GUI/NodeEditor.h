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
	static NodeEditor* s_nodeEditor;
	std::vector<int> m_selectedNodes;
public:
	//void initialize();
	static NodeEditor* getSingleton() {
		if (!s_nodeEditor)
		{
			s_nodeEditor = new NodeEditor();
		}
		return s_nodeEditor;
	}

	std::vector<int> getSelectedNodes()
	{
		return m_selectedNodes;
	}

	void draw();
};