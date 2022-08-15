#pragma once
#include<string>
class ShaderGenerator
{
private:
	std::string shader;
	static ShaderGenerator* shader_generator;
	void generate_and_set_shader();
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

	void write_shader_to_file(std::string);
};

