#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <LodePNG/lodepng.h>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#pragma once
#include "Common/logger.h"
#include "GUI/NodeGraph.h"
#include "Rendering/Renderer.h"
#include "Rendering/ShaderGenerator.h"
#include "Common/constant.h"
#include "Rendering/Materials/Material.h"
Renderer::Renderer()
{
	float l_vertices[] = {
	 1.5f,  1.5f, 1.0f,
	 1.5f, -1.5f, 1.0f,
	-1.5f, -1.5f, 1.0f,
	-1.5f,  1.5f, 1.0f
	};
	unsigned int l_indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int l_VBO;
	glGenBuffers(1, &l_VBO);

	unsigned int l_EBO;
	glGenBuffers(1, &l_EBO);

	//assemble_shader();

	setupFrameBuffer();

	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, l_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(l_vertices), l_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(l_indices), l_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Renderer::draw(float p_width, float p_height, bool from_export)
{
	if (m_cachedWidth != p_width || m_cachedHeight != p_height)
	{
		resizeRenderTexture(p_width, p_height);
	}

	if (ShaderGenerator::getSingleton()->isShaderModified())
	{
		assembleShader();
		ShaderGenerator::getSingleton()->setShaderModificationHandled();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glViewport(0.0, 0.0, p_width, p_height);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	float timeValue = glfwGetTime();
	for (Material* l_material : NodeGraph::getSingleton()->getMaterials())
	{
		l_material->setUniforms();
	}
	setRenderUniforms();
	glUniform3f(m_cameraOrigin, m_cameraOriginValue[0], m_cameraOriginValue[1], m_cameraOriginValue[2]);
	glUniform2f(m_viewportSize, p_width, p_height);
	glUniform1f(m_focalLength, m_focalLengthValue);
	glUseProgram(m_shaderProgram);
	glBindVertexArray(m_VAO);
	if(m_render_continously || from_export)
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::assembleShader()
{
	std::string l_fragmentSrc = ShaderGenerator::getSingleton()->getShader();

	std::string l_vertexSrc =
	#include "Rendering/Shaders/vertex.vs"
	"";

	unsigned int l_vertexShader = compileShader(l_vertexSrc.c_str(), GL_VERTEX_SHADER);
	unsigned int l_fragmentShader = compileShader(l_fragmentSrc.c_str(), GL_FRAGMENT_SHADER);

	linkShader(l_vertexShader, l_fragmentShader);

	glDeleteShader(l_vertexShader);
	glDeleteShader(l_fragmentShader);
}

unsigned int Renderer::compileShader(std::string p_shaderPath, unsigned int p_shaderType)
{
	std::string l_shaderString;
	std::ifstream sourceFile(p_shaderPath);
	l_shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
	const GLchar* l_shaderSource = l_shaderString.c_str();

	unsigned int l_shader;
	l_shader = glCreateShader(p_shaderType);

	glShaderSource(l_shader, 1, (const GLchar**)&l_shaderSource, NULL);
	glCompileShader(l_shader);

	int  l_success;
	char l_infoLog[512];
	glGetShaderiv(l_shader, GL_COMPILE_STATUS, &l_success);
	if (!l_success)
	{
		glGetShaderInfoLog(l_shader, 512, NULL, l_infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << l_infoLog << std::endl;
	}
	return l_shader;
}

unsigned int Renderer::compileShader(const char* p_shaderSrc, unsigned int p_shaderType)
{
	const GLchar* l_shaderSource = p_shaderSrc;

	unsigned int l_shader;
	l_shader = glCreateShader(p_shaderType);

	glShaderSource(l_shader, 1, (const GLchar**)&l_shaderSource, NULL);
	glCompileShader(l_shader);

	int  l_success;
	char l_infoLog[512];
	glGetShaderiv(l_shader, GL_COMPILE_STATUS, &l_success);
	if (!l_success)
	{
		glGetShaderInfoLog(l_shader, 512, NULL, l_infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << l_infoLog << std::endl;
		std::cout << "Shader : \n";
		std::cout << p_shaderSrc << std::endl;
	}
	return l_shader;
}


void Renderer::linkShader(unsigned int p_vertexShader, unsigned int p_fragmentShader)
{
	m_shaderProgram = glCreateProgram();

	glAttachShader(m_shaderProgram, p_vertexShader);
	glAttachShader(m_shaderProgram, p_fragmentShader);
	glLinkProgram(m_shaderProgram);

	m_cameraOrigin = glGetUniformLocation(m_shaderProgram, "u_camera_origin");
	m_viewportSize = glGetUniformLocation(m_shaderProgram, "u_viewport_size");
	m_focalLength = glGetUniformLocation(m_shaderProgram, "u_focal_length");

	int  l_success;
	char l_infoLog[512];
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &l_success);
	if (!l_success) {
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, l_infoLog);
		std::cout << "ERROR::SHADER::LINK_FAILED\n" << l_infoLog << std::endl;
	}
}

void Renderer::setupFrameBuffer()
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	glGenTextures(1, &m_renderTexture);
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::resizeRenderTexture(float p_width, float p_height)
{
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p_width, p_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_cachedWidth = p_width;
	m_cachedHeight = p_height;
}

void Renderer::exportImage(std::string p_fileName, int p_width, int p_height)
{
	std::vector<unsigned char> l_png;
	std::vector<unsigned char> l_image = getRenderedImage(p_width, p_height);

	int l_rowWidth = p_width * 4;
	for (int i = 0; i < p_height/2; i++)
	{
		for (int j = 0; j < l_rowWidth; j++)
		{
			//unsigned char temp = l_image[i * l_rowWidth + j];
			//l_image[i * l_rowWidth + j] = l_image[(p_height - i - 1) * l_rowWidth + j];
			//l_image[(p_height - i - 1) * l_rowWidth + j] = temp;
			std::swap(l_image[i * l_rowWidth + j], l_image[(p_height - i - 1) * l_rowWidth + j]);
		}
	}

	unsigned error = lodepng::encode(l_png, l_image, p_width, p_height);

	if (!error) lodepng::save_file(l_png, p_fileName);

	// If there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

std::vector<unsigned char> Renderer::getRenderedImage(int p_width, int p_height)
{
	
	auto start = std::chrono::high_resolution_clock::now();
	// We first render the image with required size.
	draw(p_width, p_height, true);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	PRINT("Time taken for render : " + std::to_string(duration.count()) + " microseconds")

	// Initializing the buffer.
	std::vector <unsigned char> l_imageVec((double)p_width * p_height * 4, ' ');

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);

	glGetTexImage(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		l_imageVec.data());

	return l_imageVec;
}

void Renderer::setUniformFloat(std::string p_uniform_name, float p_val)
{
	unsigned int l_uniform_location = glGetUniformLocation(m_shaderProgram, p_uniform_name.c_str());
	glUniform1f(l_uniform_location, p_val);
}

void Renderer::setUniformBool(std::string p_uniform_name, bool p_val)
{
	unsigned int l_uniform_location = glGetUniformLocation(m_shaderProgram, p_uniform_name.c_str());
	glUniform1i(l_uniform_location, p_val);
}

void Renderer::setUniformInt(std::string p_uniform_name, int p_val)
{
	unsigned int l_uniform_location = glGetUniformLocation(m_shaderProgram, p_uniform_name.c_str());
	glUniform1i(l_uniform_location, p_val);
}

void Renderer::setUniformFloat2(std::string p_uniform_name, float p_x, float p_y)
{
	unsigned int l_uniformLocation = glGetUniformLocation(m_shaderProgram, p_uniform_name.c_str());
	glUniform2f(l_uniformLocation, p_x, p_y);
}

void Renderer::setUniformFloat3(std::string p_uniform_name, float p_x, float p_y, float p_z)
{
	unsigned int l_uniformLocation = glGetUniformLocation(m_shaderProgram, p_uniform_name.c_str());
	glUniform3f(l_uniformLocation, p_x, p_y, p_z);
}
