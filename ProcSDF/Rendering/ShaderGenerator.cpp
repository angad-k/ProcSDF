#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Rendering/ShaderGenerator.h"
#include "Constants/constant.h"
#include "GUI/NodeGraph.h"


ShaderGenerator::ShaderGenerator() {
	ShaderGenerator::node_graph = NodeGraph::get_singleton();
	ShaderGenerator::compute_and_set_object_count();
	ShaderGenerator::generate_and_set_shader();
}

void ShaderGenerator::compute_and_set_object_count() {
	
	int object_count = 0;
	for (Node* node : ShaderGenerator::node_graph->nodes) {
		if (node->is_object_node) {
			object_count++;
			ShaderGenerator::object_id_to_node_pair.push_back(std::make_pair(object_count, node));
		}
	}

	ShaderGenerator::set_object_count(object_count);
}

void ShaderGenerator::generate_and_set_shader() {
	std::string shader_string;
	int index = 0;
	
	//shader_string.append(ShaderGenerator::fetch_file_content(shader_generation::shader_files[index++]));
	//shader_string.append(ShaderGenerator::fetch_file_content(shader_generation::shader_files[index++]));


	shader_string = ShaderGenerator::fetch_file_content("shader_reference");
	ShaderGenerator::set_shader(shader_string);
}

std::string ShaderGenerator::fetch_file_content(std::string file_name) {
	std::string file_content;
	std::string file_path = sdf::SHADER_FOLDER_PATH + file_name + sdf::SHADER_FILE_EXTENSION;
	std::ifstream sourceFile(file_path);
	file_content.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
	return file_content;
}

std::string ShaderGenerator::generate_object_functions() {

	//std::vector<std::vector<int>> graph_info = node_graph->get_topological_sorting();
	std::vector<std::vector<int>> object_node_graph = std::vector<std::vector<int>>(ShaderGenerator::object_count);

	int index = 0;

	for (auto it : ShaderGenerator::object_id_to_node_pair) {

	}
	return "";
}


