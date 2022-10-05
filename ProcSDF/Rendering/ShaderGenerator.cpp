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
	
	shader_string.append(ShaderGenerator::fetch_file_content(shader_generation::shader_files[index++]));
	shader_string.append(ShaderGenerator::fetch_file_content(shader_generation::shader_files[index++]));

	shader_string.append(ShaderGenerator::generate_object_functions());


	//shader_string = ShaderGenerator::fetch_file_content("shader_reference");
	std::cout << shader_string;
	ShaderGenerator::set_shader(shader_string);
}

std::string ShaderGenerator::fetch_file_content(std::string file_name) {
	std::string file_content = "\n";
	std::string file_path = sdf::SHADER_FOLDER_PATH + file_name + sdf::SHADER_FILE_EXTENSION;
	std::ifstream sourceFile(file_path);
	file_content.append((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
	file_content.append("\n");
	return file_content;
}

std::string ShaderGenerator::generate_object_functions() {

	std::tuple<std::vector<int>, std::map<int, int>, std::map<int, int>> graph_info = ShaderGenerator::node_graph->get_topological_sorting();
	std::vector<int> topological_sorting = std::get<0>(graph_info);
	std::map<int, int> timer_in = std::get<1>(graph_info);
	std::map<int, int> timer_out = std::get<2>(graph_info);
	std::string object_functions = "\n";

	std::cout << "tin:\n";
	for (auto it : timer_in)
		std::cout << it.first << " " << it.second << "\n";
	std::cout << "\n\n";
	std::cout << "tout:\n";
	for (auto it : timer_out)
		std::cout << it.first << " " << it.second << "\n";
	std::cout << "\n\n";

	for (auto it : ShaderGenerator::object_id_to_node_pair) {
		int object_id = it.first;
		Node* object_node = it.second;
		int t_in = 0, t_out = 0;
		std::string object = shader_generation::OBJECT_FUNCTION_TEMPLATE;
		std::string function_content;
		std::string return_variable_name;
		std::string return_statement;
		object.replace(object.find('$'), 1, std::to_string(object_id));

		// TODO : optimize using some data structures
		for (int i = 0; i < topological_sorting.size(); i++) {
			if (object_node->id == topological_sorting[i]) {
				t_in = timer_in[i];
				t_out = timer_out[i];
			}
		}

		for (int i = 0; i < topological_sorting.size(); i++) {
			// checking of the node is an ancestor of the object node in consideration
			Node* nd = ShaderGenerator::node_graph->allocated_ids[topological_sorting[i]];
			if (timer_in[nd->id] < t_in && timer_out[nd->id] > t_out) {
				
				function_content.append(nd->get_string());
				function_content.append("\n");
				return_variable_name = nd->variable_name;
			}
		}

		return_statement = shader_generation::RETURN;
		return_statement.replace(return_statement.find('$'), 1, return_variable_name);

		function_content.append(return_statement);
		object.replace(object.find('#'), 1, function_content);

		object_functions.append(object);
	}

	object_functions.append("\n");

	return object_functions;
}


