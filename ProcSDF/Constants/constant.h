#pragma once
#include <string>
#include "ImGui/imgui.h"

namespace sdf {
	const std::string SHADER_FOLDER_PATH = "Assets/Shaders/";
	const std::string SHADER_FILE_EXTENSION = ".glsl";
	const std::string VERTEX_SHADER_PATH = "Assets/Shaders/vertex.glsl";
}

namespace imgui_colors {
	const int RED = IM_COL32(255, 50, 50, 255);
	const int GREEN = IM_COL32(80, 190, 50, 255);
	const int BLUE = IM_COL32(50, 50, 255, 255);
	const int PURPLE = IM_COL32(160, 32, 240, 255);
	const int PRIMITIVE = RED;
	const int FINAL = GREEN;
	const int OPERATION = BLUE;
	const int OBJECT = PURPLE;
}