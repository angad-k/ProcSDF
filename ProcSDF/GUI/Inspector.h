#pragma once
#include "ImGui/imgui.h"
#include <stdio.h>
#include <iostream>

#include "Renderer.h"
class Inspector
{
private:
	static Inspector* inspector;

	Renderer* renderer;

	enum class Tab
	{
		CAMERA_SETTINGS,
		RENDERING_SETTINGS,
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

	void draw();
};