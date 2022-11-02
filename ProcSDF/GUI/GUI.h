#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <iostream>
#include "GUI/Inspector.h"
#include "GUI/NodeEditor.h"
#include "Rendering/Renderer.h"
class GUI
{
private:
	static GUI* s_gui;

	GLFWwindow* m_window;
	GLFWwindow* setupImguiGlfw();

	Renderer* m_renderer;
	Inspector* m_inspector;
	NodeEditor* m_nodeEditor;

	ImVec2 m_renderSceneSize;

public:
	GUI();
	~GUI();
	void initialize();

	static GUI* getSingleton() {
		if (!s_gui)
		{
			s_gui = new GUI();
		}
		return s_gui;
	}

	GLFWwindow* getWindow()
	{
		return m_window;
	}

	ImVec2 getRenderSize()
	{
		return m_renderSceneSize;
	}

	// set up the frame - sets up the GUI. doesn't render it.
	void setupFrame();

	// renders the GUI.
	void renderFrame();
};