#pragma once
#include<string>
class ShaderGenerator
{
private:
	std::string shader;
	int object_count;
	static ShaderGenerator* shader_generator;
	void generate_and_set_shader();
	static std::string fetch_file_content(std::string file_name);
	std::string generate_object_functions();
	void compute_and_set_object_count();
public:
	static ShaderGenerator* get_singleton() {
		if (!shader_generator)
		{
			shader_generator = new ShaderGenerator();
		}
		return shader_generator;
	}
	ShaderGenerator();

	std::string get_shader() {
		return shader;
	}

	void set_shader(std::string shader) {
		this->shader = shader;
	}

	int get_object_count() {
		return object_count;
	}

	void set_object_count(int object_count) {
		this->object_count = object_count;
	}

};

