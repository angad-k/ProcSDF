#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 800);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // --------------- FLOW --------------------------

    // --------------- Vertices aur triangle banane ke indices --------------------------
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

    // ek buffer generate krke uski id VBO mei store krenge
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // element buffer for not repeeating vertices.
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // --------------- VERTEX SHADER FLOW --------------------------

    //vertex shader ka source
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    //vertexShader mei id store hogi jo createShader se milegi
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // shader bana ke compile hora idhar.
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //errors check krne ke lie
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // --------------- FRAGMENT SHADER FLOW --------------------------

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
        "vec2 uv = (gl_FragCoord.xy/vec2(800.0, 800.0))*2.0 - 1.0;\n"
        "vec3 camera_position = vec3(0.0, 0.0, -5.0);\n"
        "vec3 ro = cameraPosition;\n"
        "vec3 rd = vec3(uv, 1.0);\n"

        "vec3 shaded_color = ray_march(ro, rd);\n"
        "   FragColor = vec4(shaded_color, 1.0);\n"
        "}\0";

    // similar to vertex shader ka flow
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);



    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // --------------- LINKING SHADER PROGRAM --------------------------


    //abhi shader program banayenge jo vertex and fragment use krke aage ka kaam krega
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int cameraLocation = glGetUniformLocation(shaderProgram, "cameraPosition");

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }
    // ek baari shader program ban jaaye uske baad yeh dono ni chaiye
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // --------------- VERTEX ARRAY OBJECT --------------------------

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    // VBO ko bind krke data bhej diya
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // EBO ko bind krke data bhej diya
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // vertex attribute kaise read krne ka batake attribute enable kr diya
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        float timeValue = glfwGetTime();

        glUniform3f(cameraLocation, 0.0f, 0.0f, -10.0f + 5.0 * sin(timeValue));
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}