#include "Rendering/ShaderGenerator.h"
#include "Constants/constant.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// change the order of files according to the order of insertion in the generated fragment shader file.
const std::vector<std::string> shader_files{"boilerplate", "primitive", "object_distance_functions", "ray_computation", "object_color", "ray_march", "main"};

ShaderGenerator::ShaderGenerator() {
	ShaderGenerator::generate_and_set_shader();
}

void ShaderGenerator::generate_and_set_shader() {
	std::string shader_string;
	for (std::string shader_file : shader_files) {
		std::string shader_component_string;
		std::string file_path = sdf::SHADER_FOLDER_PATH + shader_file + sdf::SHADER_FILE_EXTENSION;
		std::ifstream sourceFile(sdf::SHADER_FOLDER_PATH + shader_file + sdf::SHADER_FILE_EXTENSION);
		shader_component_string.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
		shader_string.append("\n\n" + shader_component_string);
	}

	ShaderGenerator::set_shader(shader_string);
}

