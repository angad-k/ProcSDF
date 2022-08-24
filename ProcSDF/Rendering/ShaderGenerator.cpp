#include "Rendering/ShaderGenerator.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const std::string FOLDER_PATH = "Assets/Shaders/";
const std::string FILE_EXTENSION = ".glsl";
// change the order of files according to the order of insertion in the generated fragment shader file.
const std::vector<std::string> shader_files{"boilerplate", "primitive", "object_distance_functions", "ray_computation", "object_color", "ray_march", "main"};

ShaderGenerator::ShaderGenerator() {
	ShaderGenerator::generate_and_set_shader();
}

void ShaderGenerator::generate_and_set_shader() {
	std::string shader_string;
	for (std::string shader_file : shader_files) {
		std::string shader_component_string;
		std::string file_path = FOLDER_PATH + shader_file + FILE_EXTENSION;
		std::ifstream sourceFile(FOLDER_PATH + shader_file + FILE_EXTENSION);
		shader_component_string.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
		shader_string.append("\n\n" + shader_component_string);
	}
	//std::cout << "Finished creating\n"<<shader_string<<"\n";
	ShaderGenerator::set_shader(shader_string);
}

void ShaderGenerator::write_shader_to_file(std::string file_path) {
	std::ofstream out(file_path);
	out << ShaderGenerator::get_shader();
	out.close();
}

