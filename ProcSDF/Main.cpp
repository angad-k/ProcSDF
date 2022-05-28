#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <glad/glad.h>
#include "Renderer.h"
#include "GUI/GUI.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <windows.h>

// This is just a one-liner I found online that increases the code's affinity to Nvidia's GPU and so, 
// doesn't run on the integrated card by default.
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

Renderer* Renderer::renderer = 0;
GUI* GUI::gui = 0;

int main(int, char**)
{
	//GUI needs to be created before renderer since it also sets up OpenGL
	GUI* gui = GUI::get_singleton();
	Renderer* renderer = Renderer::get_singleton();

	//This lets the GUI query for Renderer's singleton object.
	gui->initialize();
	
	GLFWwindow* window = gui->get_window();
	unsigned int render_texture = renderer->get_render_texture();
	
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
