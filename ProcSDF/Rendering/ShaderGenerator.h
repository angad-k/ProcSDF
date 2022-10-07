#pragma once
#include<string>

#include "GUI/NodeGraph.h"
class ShaderGenerator
{
private:
	std::string shader;
	int object_count;
	static ShaderGenerator* shader_generator;
	std::map<int, int> node_id_to_object_id_map;

	
	bool shader_modified = true;
	static std::string fetch_file_content(std::string file_name);
	std::string generate_object_functions();
	std::string generate_closest_object_info_function();
	std::string generate_calculate_normal_function();
	std::string generate_get_target_ray_function();
	void compute_and_set_object_count();
public:
	static ShaderGenerator* get_singleton() {
		if (!shader_generator)
		{
			shader_generator = new ShaderGenerator();
		}
		return shader_generator;
	}

	std::string get_shader() {
		return shader;
	}

	bool is_shader_modified()
	{
		return shader_modified;
	}

	void set_shader_modification_handled()
	{
		shader_modified = false;
	}

	void set_shader(std::string shader) {
		this->shader = shader;
	}

	int get_object_count() {
		return object_count;
	}

	void set_object_count(int count) {
		object_count = count;
	}

	void generate_and_set_shader();
};

