#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>

#include "Rendering/Renderer.h"
#include "Rendering/ShaderGenerator.h"
#include "Constants/constant.h"
#pragma once

Renderer::Renderer()
{
	float vertices[] = {
	 1.5f,  1.5f, 1.0f,
	 1.5f, -1.5f, 1.0f,
	-1.5f, -1.5f, 1.0f,
	-1.5f,  1.5f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	//assemble_shader();

	setup_frame_buffer();

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Renderer::draw(float width, float height)
{
	if (cachedWidth != width || cachedHeight != height)
	{
		resize_render_texture(width, height);
	}

	if (ShaderGenerator::get_singleton()->is_shader_modified())
	{
		assemble_shader();
		ShaderGenerator::get_singleton()->set_shader_modification_handled();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0.0, 0.0, width, height);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	float timeValue = glfwGetTime();

	glUniform3f(cameraOrigin, cameraOriginValue[0], cameraOriginValue[1], cameraOriginValue[2]);// +5.0 * sin(timeValue));
	glUniform2f(viewportSize, width, height);
	glUniform1f(focalLength, focalLengthValue);
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::assemble_shader()
{
	std::string fragmentSrc = ShaderGenerator::get_singleton()->get_shader();

	std::string vertexPath = sdf::VERTEX_SHADER_PATH;
	unsigned int vertexShader = compile_shader(vertexPath, GL_VERTEX_SHADER);
	unsigned int fragmentShader = compile_shader(fragmentSrc.c_str(), GL_FRAGMENT_SHADER);

	link_shader(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

unsigned int Renderer::compile_shader(std::string shaderPath, unsigned int shaderType)
{
	std::string shaderString;
	std::ifstream sourceFile(shaderPath);
	shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
	const GLchar* shaderSource = shaderString.c_str();

	unsigned int shader;
	shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

unsigned int Renderer::compile_shader(const char* shaderSrc, unsigned int shaderType)
{
	const GLchar* shaderSource = shaderSrc;

	unsigned int shader;
	shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cout << "Shader : \n";
		std::cout << shaderSrc << std::endl;
	}
	return shader;
}


void Renderer::link_shader(unsigned int vertexShader, unsigned int fragmentShader)
{
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	cameraOrigin = glGetUniformLocation(shaderProgram, "u_camera_origin");
	viewportSize = glGetUniformLocation(shaderProgram, "u_viewport_size");
	focalLength = glGetUniformLocation(shaderProgram, "u_focal_length");

	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
	}
}

void Renderer::setup_frame_buffer()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &render_texture);
	glBindTexture(GL_TEXTURE_2D, render_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::resize_render_texture(float width, float height)
{
	glBindTexture(GL_TEXTURE_2D, render_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_texture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	cachedWidth = width;
	cachedHeight = height;
}