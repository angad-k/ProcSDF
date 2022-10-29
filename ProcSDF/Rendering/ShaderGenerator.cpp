#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Rendering/Renderer.h"
#include "Rendering/ShaderGenerator.h"
#include "Constants/constant.h"
#include "GUI/NodeGraph.h"
void ShaderGenerator::compute_and_set_object_count() {
	
	int object_count = 0;
	for (Node* node : NodeGraph::get_singleton()->nodes) {
		if (node->is_object_node) {
			object_count++;
			ShaderGenerator::node_id_to_object_id_map[node->id] = object_count;
			ShaderGenerator::object_id_to_node_id_map[object_count] = node->id;
		}
	}

	ShaderGenerator::set_object_count(object_count);
}

void ShaderGenerator::compute_uniforms()
{
	uniform_vec3.clear();
	uniform_floats.clear();
	for (Node* node : NodeGraph::get_singleton()->nodes) {
		std::string variable_name = node->get_variable_name();
		for (std::string vec3_label : node->input_float3_labels)
		{
			uniform_vec3.push_back(get_uniform_string_from_label(variable_name, vec3_label));
		}
		for (std::string float_label : node->input_float_labels)
		{
			uniform_floats.push_back(get_uniform_string_from_label(variable_name, float_label));
		}
	}
}

void ShaderGenerator::generate_and_set_shader() {

	compute_and_set_object_count();
	compute_uniforms();

	std::string shader_string;
	int index = 0;
	
	// Appends the shader header which includes a number of utility functions.
	shader_string.append(ShaderGenerator::fetch_file_content(shader_generation::shader_files[index++]));
	// Appends all the uniform declarations.
	shader_string.append(ShaderGenerator::generate_uniform_declarations());
	// Appends all the primitive functions.
	shader_string.append(ShaderGenerator::fetch_file_content(shader_generation::shader_files[index++]));
	// Generates and appends the object distance functions.
	shader_string.append(ShaderGenerator::generate_object_functions());
	// Generates and appends the closest object info function.
	shader_string.append(ShaderGenerator::generate_closest_object_info_function());
	// Generates and appends the calculate normal function.
	shader_string.append(ShaderGenerator::generate_calculate_normal_function());
	// Generates and appends the get target ray function.
	shader_string.append(ShaderGenerator::generate_get_target_ray_function());
	// Appends the raymarch and main function.
	shader_string.append(ShaderGenerator::fetch_file_content(shader_generation::shader_files[index++]));

	ShaderGenerator::set_shader(shader_string);

	shader_modified = true;
}

std::string ShaderGenerator::fetch_file_content(std::string file_name) {
	std::string file_content = "\n";
	std::string file_path = sdf::SHADER_FOLDER_PATH + file_name + sdf::SHADER_FILE_EXTENSION;
	std::ifstream sourceFile(file_path);
	file_content.append((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
	file_content.append("\n");
	return file_content;
}

std::string ShaderGenerator::generate_uniform_declarations()
{
	std::string uniform_declarations = "";
	for (std::string uniform_float : uniform_floats)
	{
		std::string float_template = shader_generation::uniform::UNIFORM_FLOAT;
		float_template.replace(float_template.find('$'), 1, uniform_float);
		uniform_declarations.append(float_template);
	}

	for (std::string vec3 : uniform_vec3)
	{
		std::string vec3_template = shader_generation::uniform::UNIFORM_VEC3;
		vec3_template.replace(vec3_template.find('$'), 1, vec3);
		uniform_declarations.append(vec3_template);
	}
	
	return uniform_declarations;
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


// TODO : refactor code to have small functions
std::string ShaderGenerator::generate_object_functions() {
	
	std::string object_functions;
	NodeGraph* nodeGraph = NodeGraph::get_singleton();
	std::vector<int> topological_sorting = nodeGraph->get_topological_sorting();

	for (int node_id : topological_sorting) {
		
		Node* nd = nodeGraph->get_node(node_id);
		if (nd->is_transform_node || nd->is_final_node) {
			continue;
		}
		std::string function_name = nd->get_variable_name();
		std::string function_body = shader_generation::object_function::INITIALIZATION;
		std::string node_function = shader_generation::object_function::FUNCTION_TEMPLATE;
		std::string return_variable = nd->get_variable_name();
		std::string return_statement = shader_generation::RETURN;
		
		int index = 0;

		if (nd->is_object_node) {
			function_name = "object";
			function_name.append("_");
			function_name.append(std::to_string(ShaderGenerator::node_id_to_object_id_map[nd->id]));
		}
		
		node_function.replace(node_function.find('$'), 1, function_name);

		for (auto it : nd->operation_ordering) {
			function_body.append(shader_generation::object_function::POSITION_RESTORATION);
			for (auto itr : it.second) {
				function_body.append(ShaderGenerator::get_transform(itr));
			}
			std::string value_assignment = shader_generation::object_function::DISTANCE_STORAGE;
			if (nd->previous_non_transform_node[index] == NULL) {
				value_assignment = "\n";
				value_assignment.append(nd->get_string());
				value_assignment.append("\n");
				return_variable = nd->get_variable_name();
			}
			else {
				std::string variable_name = nd->previous_non_transform_node[index]->get_variable_name();
				while (value_assignment.find('$') != std::string::npos) {
					value_assignment.replace(value_assignment.find('$'), 1, variable_name);
				}
				return_variable = variable_name;
			}

			function_body.append(value_assignment);
			index++;
		}

		if (!nd->is_object_node) {
			return_variable = nd->get_variable_name();
		}
		return_statement.replace(return_statement.find('$'), 1, return_variable);

		if (nd->is_operation_node) {
			std::string final_statement = "\n";
			final_statement.append(nd->get_string());
			final_statement.append("\n");
			function_body.append(final_statement);
		}

		function_body.append(return_statement);
		node_function.replace(node_function.find('#'), 1, function_body);
		object_functions.append(node_function);
	}

	return object_functions;
}

std::string ShaderGenerator::get_uniform_string_from_label(std::string p_variable_name, std::string p_label){
	std::string uniform_string = "u_";
	uniform_string.append(p_variable_name);
	uniform_string.append("_");
	uniform_string.append(p_label);
	while (uniform_string.find(' ') != std::string::npos)
	{
		uniform_string.replace(uniform_string.find(' '), 1, "_");
	}
	return uniform_string;
}

std::string ShaderGenerator::get_transform(TransformNode* p_node) {
	
	std::string transform, postfix_for_rotation;
	int index = 0;

	switch (p_node->m_TransformationType) {
	case TransformationType::TRANSLATION: 
		transform = shader_generation::object_function::TRANSLATION_TRANSFORM_APPLICATION;
		break;
	case TransformationType::ROTATION_X:
		transform = shader_generation::object_function::ROTATION_TRANSFORM_INIT_X;
		postfix_for_rotation = 'x';
		break;
	case TransformationType::ROTATION_Y:
		transform = shader_generation::object_function::ROTATION_TRANSFORM_INIT_Y;
		postfix_for_rotation = 'y';
		break;
	case TransformationType::ROTATION_Z:
		transform = shader_generation::object_function::ROTATION_TRANSFORM_INIT_Z;
		postfix_for_rotation = 'z';
		break;
	}
	if (p_node->m_TransformationType == TransformationType::TRANSLATION) {
		while (transform.find('$') != std::string::npos) {
			transform.replace(
				transform.find('$'), 
				1, 
				get_uniform_string_from_label(p_node->get_variable_name(), p_node->input_float3_labels[index])
			);
			index++;
		}
	}
	else {
		while (transform.find('$') != std::string::npos) {
			transform.replace(
				transform.find('$'), 
				1, 
				get_uniform_string_from_label(p_node->get_variable_name(), p_node->input_float_labels[0])
			);
		}
		std::string transform_application = shader_generation::object_function::ROTATION_TRANSFORM_APPLICATION;
		transform_application.replace(transform_application.find('$'), 1, postfix_for_rotation);
		transform.append(transform_application);
	}

	return transform;
}


