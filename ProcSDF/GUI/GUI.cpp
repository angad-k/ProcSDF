#include "GUI/GUI.h"
#include "GUI/NodeGraph.h"
#include "Roboto.cpp"
#include "RobotoRegular.cpp"

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
#ifdef IMGUI_HAS_VIEWPORT
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
#else 
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::Begin("ProcSDF", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

		ImGui::PushFont(robotoRegularFont);

		static float w = 550.0f;
		static float h = 500.0f;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::BeginChild("Scene and Inspector", ImVec2(w, -1), true);
		{
			{
				ImGui::BeginChild("Scene", ImVec2(w, h));
				ImVec2 l_vMin = ImGui::GetWindowContentRegionMin();
				ImVec2 l_vMax = ImGui::GetWindowContentRegionMax();

				l_vMin.x += ImGui::GetWindowPos().x;
				l_vMin.y += ImGui::GetWindowPos().y;
				l_vMax.x += ImGui::GetWindowPos().x;
				l_vMax.y += ImGui::GetWindowPos().y;
				m_renderSceneSize = ImVec2(l_vMax.x - l_vMin.x, l_vMax.y - l_vMin.y);

				ImGui::Image((ImTextureID)m_renderer->getRenderTexture(), m_renderSceneSize, ImVec2(0, 1), ImVec2(1, 0));
				ImGui::EndChild();

				ImGui::Button("  ", ImVec2(w, 4.0f));
				if (ImGui::IsItemActive())
					h += ImGui::GetIO().MouseDelta.y;
			}
			
			{
				ImGui::BeginChild("Inspector", ImVec2(w, -1));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
				m_inspector->draw();
				ImGui::PopStyleVar();
				ImGui::EndChild();
			}
			
			
		}
		ImGui::EndChild();
		ImGui::SameLine();

		ImGui::Button(" ", ImVec2(4.0f, -1));
		if (ImGui::IsItemActive())
			w += ImGui::GetIO().MouseDelta.x;
		ImGui::SameLine();
		ImGui::BeginChild("Node Editor", ImVec2(0, -1), false);
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
			m_nodeEditor->draw();
			ImGui::PopStyleVar();
		}
		ImGui::EndChild();
		
		ImGui::PopStyleVar();

		ImGui::PopStyleVar();

		ImGui::PopFont();
		ImGui::End();
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

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	GLFWwindow* l_window = glfwCreateWindow(mode->width, mode->height, "ProcSDF", monitor, nullptr);
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

	setupStyle();
	robotoMediumFont = io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_compressed_data, Roboto_compressed_size, 16.0f);
	robotoRegularFont = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoRegular_compressed_data, RobotoRegular_compressed_size, 16.0f);

	ImGui_ImplGlfw_InitForOpenGL(l_window, true);
	ImGui_ImplOpenGL3_Init(l_glslVersion);

	return l_window;
}

void GUI::setupStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;

#define HI(v)   ImVec4(0.502f, 0.075f, 0.256f, v)
#define MED(v)  ImVec4(0.455f, 0.198f, 0.301f, v)
//#define MED(v)  ImVec4(0.255f, 0.098f, 0.101f, v)
#define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
	// backgrounds (@todo: complete with BG_MED, BG_LOW)
#define BG(v)   ImVec4(0.05f, 0.07f, 0.120f, v)
// text
//#define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)
#define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = TEXT(0.78f);
	style.Colors[ImGuiCol_TextDisabled] = TEXT(0.28f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = BG(0.58f);
	style.Colors[ImGuiCol_PopupBg] = BG(0.9f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = BG(1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = MED(0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = MED(1.00f);
	style.Colors[ImGuiCol_TitleBg] = LOW(1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = HI(1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = BG(0.75f);
	style.Colors[ImGuiCol_MenuBarBg] = BG(0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = BG(1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = MED(1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	//style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.27f, 0.320f, 0.5);
	style.Colors[ImGuiCol_ButtonHovered] = MED(0.86f);
	style.Colors[ImGuiCol_ButtonActive] = MED(1.00f);
	style.Colors[ImGuiCol_Header] = MED(0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = MED(0.86f);
	style.Colors[ImGuiCol_HeaderActive] = HI(1.00f);
	//style.Colors[ImGuiCol_Column] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
	//style.Colors[ImGuiCol_ColumnHovered] = MED(0.78f);
	//style.Colors[ImGuiCol_ColumnActive] = MED(1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = MED(1.00f);
	style.Colors[ImGuiCol_PlotLines] = TEXT(0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = MED(1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = TEXT(0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = MED(1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = MED(0.43f);
	// [...]
	style.Colors[ImGuiCol_ModalWindowDimBg] = BG(0.73f);

	style.WindowPadding = ImVec2(6, 4);
	style.WindowRounding = 0.0f;
	style.FramePadding = ImVec2(5, 2);
	style.FrameRounding = 0.0f;
	style.ItemSpacing = ImVec2(10, 10);
	style.ItemInnerSpacing = ImVec2(1, 1);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 6.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 2.0f;
	style.WindowTitleAlign.x = 0.50f;
	
	style.WindowRounding = 1;
	style.ChildRounding = 1;
	style.FrameRounding = 1;
	style.PopupRounding = 1;

	style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
	style.WindowBorderSize = 1.0f;
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