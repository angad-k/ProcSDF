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

#define HI(v)   ImVec4(0.502f, 0.075f, 0.256f, v)
#define MED(v)  ImVec4(0.455f, 0.198f, 0.301f, v)
#define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
#define BG(v)   ImVec4(0.05f, 0.07f, 0.120f, v)
#define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

class GUI
{
private:
	static GUI* s_gui;

	GLFWwindow* m_window;
	GLFWwindow* setupImguiGlfw();

	void setupStyle();

	Renderer* m_renderer;
	Inspector* m_inspector;
	NodeEditor* m_nodeEditor;

	ImVec2 m_renderSceneSize;

	ImFont* defaultFont;
	ImFont* robotoMediumFont;
	ImFont* robotoRegularFont;

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

	void pushMediumFont()
	{
		ImGui::PushFont(robotoMediumFont);
	}
	void pushDefaultFont()
	{
		ImGui::PushFont(defaultFont);
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