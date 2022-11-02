#include "GUI/GUI.h"
#include "GUI/NodeGraph.h"

static void glfwErrorCallback(int p_error, const char* p_description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", p_error, p_description);
}

GUI::GUI()
{
	m_window = setupImguiGlfw();
	if (m_window == NULL)
	{
		std::cout << "ERROR::GUI::FAILED TO INITIALIZE WINDOW\n";
	}
}

void GUI::initialize()
{
	m_renderer = Renderer::getSingleton();
	
	//All new windows added to GUI need to be initialized after being created.
	m_inspector = Inspector::getSingleton();
	m_inspector->initialize();

	m_nodeEditor = NodeEditor::getSingleton();
	//nodeEditor->initialize();

	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
	l_nodeGraph->initialize();

}

void GUI::setupFrame()
{
	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	{
		{
			ImGui::Begin("Scene");

			ImVec2 l_vMin = ImGui::GetWindowContentRegionMin();
			ImVec2 l_vMax = ImGui::GetWindowContentRegionMax();

			l_vMin.x += ImGui::GetWindowPos().x;
			l_vMin.y += ImGui::GetWindowPos().y;
			l_vMax.x += ImGui::GetWindowPos().x;
			l_vMax.y += ImGui::GetWindowPos().y;
			m_renderSceneSize = ImVec2(l_vMax.x - l_vMin.x, l_vMax.y - l_vMin.y);

			ImGui::Image((ImTextureID)m_renderer->getRenderTexture(), m_renderSceneSize, ImVec2(0, 1), ImVec2(1, 0));

			ImGui::End();
		}

		{
			m_inspector->draw();
		}

		{
			m_nodeEditor->draw();
		}
	}
}

void GUI::renderFrame()
{
	ImGui::Render();
	int l_displayW, l_displayH;
	glfwGetFramebufferSize(m_window, &l_displayW, &l_displayH);
	glViewport(0, 0, l_displayW, l_displayH);
	ImVec4 l_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	glClearColor(l_clearColor.x * l_clearColor.w, l_clearColor.y * l_clearColor.w, l_clearColor.z * l_clearColor.w, l_clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(m_window);
}

GLFWwindow* GUI::setupImguiGlfw()
{
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit())
		return NULL;

	const char* l_glslVersion = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* l_window = glfwCreateWindow(1280, 720, "ProcSDF", NULL, NULL);
	if (l_window == NULL)
		return NULL;
	glfwMakeContextCurrent(l_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImNodes::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigDragClickToInputText = true;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(l_window, true);
	ImGui_ImplOpenGL3_Init(l_glslVersion);

	return l_window;
}

GUI::~GUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	ImNodes::DestroyContext();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}