#include "Rendering/ShaderGenerator.h"
#include "Constants/constant.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

ShaderGenerator::ShaderGenerator() {
	ShaderGenerator::generate_and_set_shader();
}

void ShaderGenerator::generate_and_set_shader() {
	std::string shader_string;

	ShaderGenerator::set_shader(shader_string);
}

std::string fetch_file_content(std::string file_name) {
	std::string file_content;
	std::string file_path = sdf::SHADER_FOLDER_PATH + file_name + sdf::SHADER_FILE_EXTENSION;
	std::ifstream sourceFile(file_path);
	file_content.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
	return file_content;
}


