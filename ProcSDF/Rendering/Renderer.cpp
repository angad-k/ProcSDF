#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Renderer
{
public :    
    unsigned int render_texture;
    unsigned int VAO;
    unsigned int shaderProgram;
    unsigned int FBO;
    int cameraLocation;
    Renderer()
    {
        float vertices[] = {
         1.5f,  1.5f, 1.0f,  // top right
         1.5f, -1.5f, 1.0f,  // bottom right
        -1.5f, -1.5f, 1.0f,  // bottom left
        -1.5f,  1.5f, 1.0f   // top left 
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };

        unsigned int VBO;
        glGenBuffers(1, &VBO);

        unsigned int EBO;
        glGenBuffers(1, &EBO);

        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";

        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }


        const char* fragmentShaderSource = "#version 330 core\n"
            "float distance_from_sphere(in vec3 p, in vec3 c, float r)\n"
            "{\n"
            "return length(p - c) - r;\n"
            "}\n"
            "    float map_the_world(in vec3 p)\n"
            "{\n"
            "float displacement = sin(5.0 * p.x) * sin(5.0 * p.y) * sin(5.0 * p.z) * 0.25;\n"
            "     float sphere_0 = distance_from_sphere(p, vec3(0.0), 1.0);\n"
            "\n"
            "     return sphere_0+ displacement;\n"
            "  }\n"
            "\n"
            "  vec3 calculate_normal(in vec3 p)\n"
            "  {\n"
            "      const vec3 small_step = vec3(0.001, 0.0, 0.0);\n"
            "\n"
            "      float gradient_x = map_the_world(p + small_step.xyy) - map_the_world(p - small_step.xyy);\n"
            "     float gradient_y = map_the_world(p + small_step.yxy) - map_the_world(p - small_step.yxy);\n"
            "       float gradient_z = map_the_world(p + small_step.yyx) - map_the_world(p - small_step.yyx);\n"
            "\n"
            "      vec3 normal = vec3(gradient_x, gradient_y, gradient_z);\n"
            "\n"
            "      return normalize(normal);\n"
            "   }\n"
            "\n"
            "   vec3 ray_march(in vec3 ro, in vec3 rd)\n"
            "{\n"
            "       float total_distance_traveled = 0.0;\n"
            "      const int NUMBER_OF_STEPS = 32;\n"
            "     const float MINIMUM_HIT_DISTANCE = 0.001;\n"
            "  const float MAXIMUM_TRACE_DISTANCE = 1000.0;\n"
            "\n"
            "     for (int i = 0; i < NUMBER_OF_STEPS; ++i)\n"
            "    {\n"
            "        vec3 current_position = ro + total_distance_traveled * rd;\n"
            "\n"
            "        float distance_to_closest = map_the_world(current_position);\n"
            "\n"
            "       if (distance_to_closest < MINIMUM_HIT_DISTANCE)\n"
            "       {\n"
            "           vec3 normal = calculate_normal(current_position);\n"
            "           vec3 light_position = vec3(2.0, -5.0, 3.0);\n"
            "           vec3 direction_to_light = normalize(current_position - light_position);\n"
            "\n"
            " float diffuse_intensity = max(0.0, dot(normal, direction_to_light));\n"
            "\n"
            "          return vec3(1.0, 0.0, 0.0) * diffuse_intensity;\n"
            "  }\n"
            "\n"
            "        if (total_distance_traveled > MAXIMUM_TRACE_DISTANCE)\n"
            "        {\n"
            "            break;\n"
            "        }\n"
            "        total_distance_traveled += distance_to_closest;\n"
            "    }\n"
            "   return vec3(0.0);\n"
            "}\n"
            "out vec4 FragColor;\n"
            "uniform vec3 cameraPosition; \n"
            "void main()\n"
            "{\n"
            "vec2 uv = (gl_FragCoord.xy/vec2(600.0, 600.0))*2.0 - 1.0;\n"
            "vec3 camera_position = vec3(0.0, 0.0, -5.0);\n"
            "vec3 ro = cameraPosition;\n"
            "vec3 rd = vec3(uv, 1.0);\n"

            "vec3 shaded_color = ray_march(ro, rd);\n"
            "   FragColor = vec4(shaded_color, 1.0);\n"
            "}\0";

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glGenTextures(1, &render_texture);
        glBindTexture(GL_TEXTURE_2D, render_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_texture, 0);

        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 600, 600);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        cameraLocation = glGetUniformLocation(shaderProgram, "cameraPosition");

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void draw()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glViewport(0.0, 0.0, 600, 600);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        float timeValue = glfwGetTime();

        glUniform3f(cameraLocation, 0.0f, 0.0f, -10.0f + 5.0 * sin(timeValue));
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};
