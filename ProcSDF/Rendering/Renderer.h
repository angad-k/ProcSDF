#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
class Renderer
{
private:
	unsigned int render_texture;
	unsigned int VAO;
	unsigned int shaderProgram;
	unsigned int FBO;

	float cachedWidth = 600;
	float cachedHeight = 600;

	int cameraOrigin;
	int focalLength;
	int viewportSize;
	int lightPosition;

	float cameraOriginValue[3] = { 0.0, 0.0, 0.0 };
	float focalLengthValue = 1.0;
	float lightPositionValue[3] = { 2.0, 5.0, -3.0 };

	void assemble_shader();
	unsigned int compile_shader(std::string shaderPath, unsigned int shaderType);
	void link_shader(unsigned int vertexShader, unsigned int fragmentShader);
	void setup_frame_buffer();
	void resize_render_texture(float height, float width);
public:
	Renderer();
	void draw(float width, float height);
	unsigned int get_render_texture()
	{
		return render_texture;
	}

	void set_camera_origin(float p_cameraOrigin[3])
	{
		cameraOriginValue[0] = p_cameraOrigin[0];
		cameraOriginValue[1] = p_cameraOrigin[1];
		cameraOriginValue[2] = p_cameraOrigin[2];
	}

	float* get_camera_origin()
	{
		return cameraOriginValue;
	}

	void set_focal_length(float p_focalLength)
	{
		focalLengthValue = p_focalLength;
	}

	float* get_focal_length()
	{
		return &focalLengthValue;
	}

	float* get_light_position()
	{
		return lightPositionValue;
	}
};