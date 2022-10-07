#pragma once
#include "ImGui/imgui.h"
#include <stdio.h>
#include <iostream>
#include "GUI/Nodes/Node.h"
#include "Rendering/Renderer.h"
class Inspector
{
private:
	static Inspector* inspector;

	Renderer* renderer;

	enum class Tab
	{
		CAMERA_SETTINGS,
		RENDERING_SETTINGS,
		NODEGRAPH_SETTINGS,
	};

	Tab openedTab;

public:
	void initialize();

	static Inspector* get_singleton() {
		if (!inspector)
		{
			inspector = new Inspector();
		}
		return inspector;
	}
	template <typename p_nodeType> Node* add_node();
	void draw();
	void draw_camera_settings();
	void draw_rendering_settings();
	void draw_node_graph_settings();
};