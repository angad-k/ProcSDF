#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <iostream>
#include "Inspector.h"
#include "NodeEditor.h"
#include "Renderer.h"
class GUI
{
private:
	static GUI* gui;

	GLFWwindow* window;
	GLFWwindow* setup_imgui_glfw();

	Renderer* renderer;
	Inspector* inspector;
	NodeEditor* nodeEditor;

	ImVec2 renderSceneSize;

public:
	GUI();
	~GUI();
	void initialize();

	static GUI* get_singleton() {
		if (!gui)
		{
			gui = new GUI();
		}
		return gui;
	}

	GLFWwindow* get_window()
	{
		return window;
	}

	ImVec2 get_render_size()
	{
		return renderSceneSize;
	}

	// set up the frame - sets up the GUI. doesn't render it.
	void setup_frame();

	// renders the GUI.
	void render_frame();
};