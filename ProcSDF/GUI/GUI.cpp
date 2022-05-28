#include "GUI.h"

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GUI::GUI()
{
	window = setup_imgui_glfw();
	if (window == NULL)
	{
		std::cout << "ERROR::GUI::FAILED TO INITIALIZE WINDOW\n";
	}
}

void GUI::initialize()
{
	renderer = Renderer::get_singleton();
}

void GUI::setup_frame()
{
	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	{
		{
			ImGui::Begin("Scene");

			ImVec2 vMin = ImGui::GetWindowContentRegionMin();
			ImVec2 vMax = ImGui::GetWindowContentRegionMax();

			vMin.x += ImGui::GetWindowPos().x;
			vMin.y += ImGui::GetWindowPos().y;
			vMax.x += ImGui::GetWindowPos().x;
			vMax.y += ImGui::GetWindowPos().y;
			renderSceneSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);

			ImGui::Image((ImTextureID)renderer->get_render_texture(), renderSceneSize, ImVec2(0, 1), ImVec2(1, 0));

			ImGui::End();
		}

		{
			ImGui::Begin("Camera");

			float origin[3] = { renderer->get_camera_origin()[0], renderer->get_camera_origin()[1], renderer->get_camera_origin()[2] };
			ImGui::SliderFloat3("Camera Origin", origin, -10.0, 10.0);
			renderer->set_camera_origin(origin);

			//float focalLength = renderer->get_focal_length();
			ImGui::SliderFloat("Focal Length", renderer->get_focal_length(), -10.0, 10.0);

			ImGui::End();
		}
	}
}

void GUI::render_frame()
{
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
}

GLFWwindow* GUI::setup_imgui_glfw()
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return NULL;

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL)
		return NULL;
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	return window;
}

GUI::~GUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}