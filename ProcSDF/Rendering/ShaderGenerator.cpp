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
			ShaderGenerator::node_id_to_object_id_map[node->id] = object_count;
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

	shader_string.append(ShaderGenerator::generate_closest_object_info_function());

	shader_string.append(ShaderGenerator::generate_calculate_normal_function());

	shader_string.append(ShaderGenerator::generate_get_target_ray_function());

	shader_string.append(ShaderGenerator::fetch_file_content(shader_generation::shader_files[index++]));

	//shader_string = ShaderGenerator::fetch_file_content("shader_reference");
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

std::string ShaderGenerator::generate_get_target_ray_function() {
	
	std::string target_ray_function = "\n";
	target_ray_function.append(shader_generation::target_ray::FUNCTION_TEMPLATE);
	target_ray_function.append("\n");

	std::string switch_statement = shader_generation::SWITCH_STATEMENT, switch_content, case_statement;

	for (int i = 1; i <= ShaderGenerator::object_count; i++) {
		case_statement = shader_generation::target_ray::CASE_STATEMENT;
		case_statement.replace(case_statement.find('$'), 1, std::to_string(i));
		switch_content.append(case_statement);
	}

	switch_statement.replace(switch_statement.find('$'), 1, switch_content);
	target_ray_function.replace(target_ray_function.find('$'), 1, switch_statement);

	return target_ray_function;

}

std::string ShaderGenerator::generate_calculate_normal_function() {
	
	std::string calculate_normal_function = "\n";
	calculate_normal_function.append(shader_generation::calculate_normal::FUNCTION_TEMPLATE);
	calculate_normal_function.append("\n");

	std::string switch_statement = shader_generation::SWITCH_STATEMENT, switch_content, case_statement;

	for (int i = 1; i <= ShaderGenerator::object_count; i++) {
		case_statement = shader_generation::calculate_normal::CASE_STATEMENT;
		for (int j = 0; j < shader_generation::calculate_normal::FREQUENCY; j++) {
			case_statement.replace(case_statement.find('$'), 1, std::to_string(i));
		}

		switch_content.append(case_statement);
	}

	switch_statement.replace(switch_statement.find('$'), 1, switch_content);
	calculate_normal_function.replace(calculate_normal_function.find('$'), 1, switch_statement);

	return calculate_normal_function;
}

std::string ShaderGenerator::generate_closest_object_info_function() {
	
	std::string closest_object_function = "\n";
	closest_object_function.append(shader_generation::closest_object_info::FUNCTION_TEMPLATE);
	closest_object_function.append("\n");

	std::string function_content,helper_string;

	for (int i = 1; i <= ShaderGenerator::object_count; i++) {
		helper_string = shader_generation::closest_object_info::DISTANCE_COMPUTATION;
		helper_string.replace(helper_string.find('$'), 1, std::to_string(i));
		helper_string.replace(helper_string.find('$'), 1, std::to_string(i));
		function_content.append(helper_string);
	}

	function_content.append(shader_generation::closest_object_info::VARIABLE_INITIALIZATION);

	for (int i = 1; i <= ShaderGenerator::object_count; i++) {
		helper_string = shader_generation::closest_object_info::MIN_DIST_COMPUTATION;
		helper_string.replace(helper_string.find('$'), 1, std::to_string(i));
		function_content.append(helper_string);
	}

	for (int i = 1; i <= ShaderGenerator::object_count; i++) {
		helper_string = shader_generation::closest_object_info::CONDITIONAL_OBJECT_INDEX_COMPUTATION;
		helper_string.replace(helper_string.find('$'), 1, std::to_string(i));
		helper_string.replace(helper_string.find('$'), 1, std::to_string(i));
		function_content.append(helper_string);
	}

	closest_object_function.replace(closest_object_function.find('$'), 1, function_content);

	return closest_object_function;
}

std::string ShaderGenerator::generate_object_functions() {

	std::vector<int> topological_sorting = ShaderGenerator::node_graph->get_topological_sorting();
	std::vector<std::vector<int>> object_info = std::vector<std::vector<int>>(ShaderGenerator::object_count, std::vector<int>());
	std::string object_functions = "\n";

	for (int node : topological_sorting) {
		for (int object_id : node_graph->reachable_objects[node]) {
			if (node != object_id) {
				object_info[ShaderGenerator::node_id_to_object_id_map[object_id] - 1].push_back(node);
			}
		}
	}

	for (int i = 0; i < object_info.size(); i++) {
		int object_id = i+1;
		std::string object = shader_generation::OBJECT_FUNCTION_TEMPLATE;
		std::string function_content;
		std::string return_variable_name;
		std::string return_statement;
		object.replace(object.find('$'), 1, std::to_string(object_id));

		for (int j = 0; j < object_info[i].size(); j++) {

			Node* nd = node_graph->allocated_ids[object_info[i][j]];
			function_content.append(nd->get_string());
			function_content.append("\n");
			return_variable_name = nd->variable_name;
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


