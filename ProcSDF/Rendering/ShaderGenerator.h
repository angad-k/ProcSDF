#pragma once
#include<string>
class ShaderGenerator
{
private:
	std::string shader;

	void generate_and_set_shader();
public:

	ShaderGenerator();

	std::string get_shader() {
		return shader;
	}

	void set_shader(std::string shader) {
		this->shader = shader;
	}

	void write_shader_to_file(std::string);
};

