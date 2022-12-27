#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include "ShaderGenerator.h"
class Renderer
{
private:
	static Renderer* s_renderer;
	unsigned int m_renderTexture;
	unsigned int m_VAO;
	unsigned int m_shaderProgram;
	unsigned int m_FBO;

	float m_cachedWidth = 600;
	float m_cachedHeight = 600;

	int m_cameraOrigin;
	int m_focalLength;
	int m_viewportSize;
	int m_lightPosition;

	float m_cameraOriginValue[3] = { 0.0, 0.0, 5.0 };
	float m_focalLengthValue = 1.0;

	void assembleShader();
	unsigned int compileShader(std::string p_shaderPath, unsigned int p_shaderType);
	unsigned int compileShader(const char* p_shaderSrc, unsigned int p_shaderType);
	void linkShader(unsigned int p_vertexShader, unsigned int p_fragmentShader);
	void setupFrameBuffer();
	void resizeRenderTexture(float p_height, float p_width);
	//String generate_fragment_shader();
	std::vector <std::string> m_render_uniforms = { "Max Depth", "Samples", "Number of steps", "Maximum Trace Distance"};
	std::vector <std::string> m_render_uniforms_debug = { "DEBUG", "DEBUG_DEPTH", "DEBUG_MAX_TRACE", "DEBUG_STEPS_END" };

public:
	std::vector <int> m_render_uniforms_values = { 16, 10, 16, 1000 };
	std::vector <bool> m_render_uniforms_debug_values = { false, false, false, false };
	std::vector<std::vector<float>> m_render_uniform_debug_cols = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
	float m_horizon_top_color[3] = { 1.0, 1.0, 1.0 };
	float m_horizon_bottom_color[3] = { 0.5, 0.7, 1.0 };
	Renderer();
	
	static Renderer* getSingleton() {
		if (!s_renderer)
		{
			s_renderer = new Renderer();
		}
		return s_renderer;
	}

	void draw(float p_width, float p_height);

	void setRenderUniforms()
	{
		for (int i = 0; i < m_render_uniforms.size(); i++)
		{
			setUniformInt(
				ShaderGenerator::getUniformStringFromLabel("r", m_render_uniforms[i]),
				Renderer::getSingleton()->m_render_uniforms_values[i]
			);
		}
		for (int i = 0; i < m_render_uniforms_debug.size(); i++)
		{
			setUniformBool(
				m_render_uniforms_debug[i],
				m_render_uniforms_debug_values[i]
			);
		}
		for (int i = 1; i < m_render_uniforms_debug.size(); i++)
		{
			setUniformFloat3(
				m_render_uniforms_debug[i] + "_COL",
				m_render_uniform_debug_cols[i-1][0],
				m_render_uniform_debug_cols[i-1][1],
				m_render_uniform_debug_cols[i-1][2]
			);
		}
		setUniformFloat3(
			"u_r_horizon_top_color",
			m_horizon_top_color[0],
			m_horizon_top_color[1],
			m_horizon_top_color[2]
		);
		setUniformFloat3(
			"u_r_horizon_bottom_color",
			m_horizon_bottom_color[0],
			m_horizon_bottom_color[1],
			m_horizon_bottom_color[2]
		);
	}

	unsigned int getRenderTexture()
	{
		return m_renderTexture;
	}

	std::vector <std::string> getRenderUniforms()
	{
		return m_render_uniforms;
	}

	std::vector <std::string> getRenderUniformsDebug()
	{
		return m_render_uniforms_debug;
	}

	void setCameraOrigin(float p_cameraOrigin[3])
	{
		m_cameraOriginValue[0] = p_cameraOrigin[0];
		m_cameraOriginValue[1] = p_cameraOrigin[1];
		m_cameraOriginValue[2] = p_cameraOrigin[2];
	}

	float* getCameraOrigin()
	{
		return m_cameraOriginValue;
	}

	void setFocalLength(float p_focalLength)
	{
		m_focalLengthValue = p_focalLength;
	}

	float* getFocalLength()
	{
		return &m_focalLengthValue;
	}
	void setUniformInt(std::string p_uniform_name, int p_val);
	void setUniformBool(std::string p_uniform_name, bool p_val);
	void setUniformFloat(std::string p_uniform_name, float p_val);
	void setUniformFloat2(std::string p_uniform_name, float p_x, float p_y);
	void setUniformFloat3(std::string p_uniform_name, float p_x, float p_y, float p_z);
	std::vector<unsigned char> getRenderedImage(int p_width, int p_height);
	void exportImage(std::string p_fileName, int p_width, int p_height);
};
