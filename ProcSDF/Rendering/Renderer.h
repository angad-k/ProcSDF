#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
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
public:
	Renderer();

	static Renderer* getSingleton() {
		if (!s_renderer)
		{
			s_renderer = new Renderer();
		}
		return s_renderer;
	}

	void draw(float p_width, float p_height);

	unsigned int getRenderTexture()
	{
		return m_renderTexture;
	}

	void setCameraOrigin(float p_cameraOrigin[3])
	{
		m_cameraOriginValue[0] = p_cameraOrigin[0];
		m_cameraOriginValue[1] = p_cameraOrigin[1];
		m_cameraOriginValue[2] = p_cameraOrigin[2];
	}

	float* get_camera_origin()
	{
		return m_cameraOriginValue;
	}

	void set_focal_length(float p_focalLength)
	{
		m_focalLengthValue = p_focalLength;
	}

	float* getFocalLength()
	{
		return &m_focalLengthValue;
	}

	void setUniformFloat(std::string p_uniform_name, float p_val);
	void setUniformFloat2(std::string p_uniform_name, float p_x, float p_y);
	void setUniformFloat3(std::string p_uniform_name, float p_x, float p_y, float p_z);
};
