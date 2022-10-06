#pragma once
#include <string>
#include<vector>

#include "ImGui/imgui.h"

namespace sdf {
	const std::string SHADER_FOLDER_PATH = "Assets/Shaders/";
	const std::string SHADER_FILE_EXTENSION = ".glsl";
	const std::string VERTEX_SHADER_PATH = "Assets/Shaders/vertex.glsl";
}

namespace shader_generation {
	// change the order of files according to the order of insertion in the generated fragment shader file.
	const std::vector<std::string> shader_files{ "header", "primitive", "footer" };
	const std::string POSITION = "position";
	const std::string OBJECT_FUNCTION_TEMPLATE = "\nfloat object_$(vec3 position)\n{\n#\n}\n";
	const std::string RETURN = "return $;\n";
	const std::string FLOAT = "float ";
	namespace closest_object_info {
		const std::string FUNCTION_TEMPLATE = "\nclosest_object_info get_closest_object_info(vec3 position)\n{\n$\nreturn closest_object_info(min_dist, object_index);\n}\n";
		const std::string DISTANCE_COMPUTATION = "\nfloat dist_$ = object_$(position);\n";
		const std::string VARIABLE_INITIALIZATION = "\nfloat min_dist = 3e+38;\nint object_index = 1;\n";
		const std::string MIN_DIST_COMPUTATION = "\nmin_dist = min(min_dist, dist_$);\n";
		const std::string CONDITIONAL_OBJECT_INDEX_COMPUTATION = "\nif(dist_$ == min_dist)\n{\nobject_index = $;\n}\n";
	}
	
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