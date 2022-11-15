#include <glad/glad.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#pragma once
#include <windows.h>
#include "Renderer.h"
#include "GUI/GUI.h"
#include "Rendering/ShaderGenerator.h"
#include "GUI/NodeGraph.h"

// This is just a one-liner I found online that increases the code's affinity to Nvidia's GPU and so, 
// doesn't run on the integrated card by default.
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

Renderer* Renderer::s_renderer = 0;
GUI* GUI::s_gui = 0;
Inspector* Inspector::s_inspector = 0;
NodeEditor* NodeEditor::s_nodeEditor = 0;
ShaderGenerator* ShaderGenerator::s_shaderGenerator = 0;
NodeGraph* NodeGraph::s_nodeGraph = 0;

int main(int, char**)
{
	//GUI needs to be created before renderer since it also sets up OpenGL
	GUI* l_gui = GUI::getSingleton();
	Renderer* l_renderer = Renderer::getSingleton();
	ShaderGenerator::getSingleton();
	NodeGraph::getSingleton();
	Inspector::getSingleton();
	NodeEditor::getSingleton();

	//This lets the GUI query for Renderer's singleton object.
	l_gui->initialize();
	
	GLFWwindow* l_window = l_gui->getWindow();
	unsigned int l_renderTexture = l_renderer->getRenderTexture();
	
	NodeGraph::getSingleton()->recompileNodeGraph();

	while (!glfwWindowShouldClose(l_window))
	{
		l_gui->setupFrame();

		ImVec2 l_renderSceneSize = l_gui->getRenderSize();
		l_renderer->draw(l_renderSceneSize.x, l_renderSceneSize.y);
		
		l_gui->renderFrame();
	}

	l_gui->~GUI();

	return 0;
}
