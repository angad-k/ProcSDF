#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>

class Renderer
{
private :
    unsigned int render_texture;
    unsigned int VAO;
    unsigned int shaderProgram;
    unsigned int FBO;
    float cachedWidth = 600;
    float cachedHeight = 600;
    int cameraOrigin;
    int focalLength;
    int viewportSize;
    void assemble_shader();
    unsigned int compile_shader(std::string shaderPath, unsigned int shaderType);
    void link_shader(unsigned int vertexShader, unsigned int fragmentShader);
    void setup_frame_buffer();
    void resize_render_texture(float height, float width);
public :
    Renderer();
    void draw(float width, float height);
    unsigned int get_render_texture()
    {
        return render_texture;
    }
};