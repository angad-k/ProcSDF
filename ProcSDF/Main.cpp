#include <glad/glad.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
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

Renderer* Renderer::renderer = 0;
GUI* GUI::gui = 0;
Inspector* Inspector::inspector = 0;
NodeEditor* NodeEditor::nodeEditor = 0;
ShaderGenerator* ShaderGenerator::shader_generator = 0;
NodeGraph* NodeGraph::nodeGraph = 0;

int main(int, char**)
{
	//GUI needs to be created before renderer since it also sets up OpenGL
	GUI* gui = GUI::get_singleton();
	Renderer* renderer = Renderer::get_singleton();
	ShaderGenerator::get_singleton();
	NodeGraph::get_singleton();
	Inspector::get_singleton();
	NodeEditor::get_singleton();

	//This lets the GUI query for Renderer's singleton object.
	gui->initialize();
	
	GLFWwindow* window = gui->get_window();
	unsigned int render_texture = renderer->get_render_texture();
	
	NodeGraph::get_singleton()->recompile_node_graph();

	while (!glfwWindowShouldClose(window))
	{
		gui->setup_frame();

		ImVec2 renderSceneSize = gui->get_render_size();
		renderer->draw(renderSceneSize.x, renderSceneSize.y);
		
		gui->render_frame();
	}

	gui->~GUI();

	return 0;
}
