#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"
#include <stdio.h>
#include <iostream>
#include "Renderer.h"
class NodeEditor
{
private:
	static NodeEditor* nodeEditor;

public:
	void initialize();

	static NodeEditor* get_singleton() {
		if (!nodeEditor)
		{
			nodeEditor = new NodeEditor();
		}
		return nodeEditor;
	}

	void draw();
};