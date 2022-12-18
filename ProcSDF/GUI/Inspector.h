#pragma once
#include "ImGui/imgui.h"
#include <stdio.h>
#include <iostream>
#include "GUI/Nodes/Node.h"
#include "Rendering/Renderer.h"
class Inspector
{
private:
	static Inspector* s_inspector;

	Renderer* m_renderer;

	enum class Tab
	{
		CAMERA_SETTINGS,
		RENDERING_SETTINGS,
		NODEGRAPH_SETTINGS,
		MATERIAL_SETTINGS,
	};

	Tab m_openedTab;

public:
	void initialize();

	static Inspector* getSingleton() {
		if (!s_inspector)
		{
			s_inspector = new Inspector();
		}
		return s_inspector;
	}
	template <typename p_nodeType> Node* addNode();
	void draw();
	void drawCameraSettings();
	void drawRenderingSettings();
	void drawNodeGraphSettings();
	void drawMaterialSettings();
};