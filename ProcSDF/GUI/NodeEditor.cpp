#include "GUI/NodeEditor.h"
#include "GUI/Nodes/Node.h"
#include "GUI/NodeGraph.h"
#include "Common/constant.h"
#include "Common/ProjectSaver.h"
#include "Common/logger.h"
#include "GUI.h"

void NodeEditor::draw()
{
	ImGui::BeginChild("Nodes workspace");

	ImGui::Dummy(ImVec2(0, 0));

	ImGui::Dummy(ImVec2(0, 0));
	ImGui::SameLine();

	if (ImGui::Button("Load Project"))
	{
		bool isParseSucessful = ProjectSaver::loadProject();
		if (!isParseSucessful) {
			ERR("Error in parsing uploaded file");
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Save Project"))
	{
		ProjectSaver::saveProject();
	}

	ImGui::SameLine();

	if ((!NodeGraph::getSingleton()->checkCompilationError()) && NodeGraph::getSingleton()->isDirty())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, HI(1.0));
	}
	if (ImGui::Button("Recompile"))
	{
		NodeGraph::getSingleton()->recompileNodeGraph();
	}
	if ((!NodeGraph::getSingleton()->checkCompilationError()) && NodeGraph::getSingleton()->isDirty())
	{
		ImGui::PopStyleColor();
	}

	ImGui::SameLine();

	if (m_selectedNodes.size() == 0)
	{
		ImGui::BeginDisabled();
	}
	ImGui::PushStyleColor(ImGuiCol_Button, imgui_colors::RED);
	if (ImGui::Button("Delete selected nodes"))
	{
		for (int sel_id : m_selectedNodes)
		{
			NodeGraph::getSingleton()->deleteNode(sel_id);
		}
	}
	ImGui::PopStyleColor();
	ImGui::SameLine();
	if (m_selectedNodes.size() == 0)
	{
		ImGui::EndDisabled();
	}
	m_selectedNodes.clear();

	ImGui::PushStyleColor(ImGuiCol_Text, imgui_colors::RED);
	ImGui::Text(NodeGraph::getSingleton()->getCompilationError().c_str());
	ImGui::PopStyleColor();
	ImGui::PushStyleColor(ImGuiCol_Text, imgui_colors::ORANGE);
	if ((!NodeGraph::getSingleton()->checkCompilationError()) && NodeGraph::getSingleton()->isDirty())
	{
		ImGui::SameLine();
		ImGui::Text("Node Graph modified, recompile for changes to take effect.");
	}
	ImGui::PopStyleColor();
	// ImNodes workspace starts from here.

	ImGui::SameLine();
	
	ImGuiStyle style = ImGui::GetStyle();
	float widthNeeded = ImGui::CalcTextSize("Quit").x + style.FramePadding.x * 2.f;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
	ImGui::PushStyleColor(ImGuiCol_Button, imgui_colors::RED);
	if (ImGui::Button("Quit"))
	{
		ImGui::OpenPopup("Confirmation");
	}
	ImGui::PopStyleColor();
	if (ImGui::BeginPopupModal("Confirmation", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		const char confString[] = "Are you sure you want to quit? Unsaved changes would be lost.";
		ImGui::Text(confString);

		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		float widthNeeded = ImGui::CalcTextSize("Yes").x + style.FramePadding.x * 2.f + ImGui::CalcTextSize("No").x + 5.0f;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::CalcTextSize(confString).x - widthNeeded);

		if (ImGui::Button("Yes"))
		{
			glfwSetWindowShouldClose(GUI::getSingleton()->getWindow(), GL_TRUE);
			
		}
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(5.0f, 5.0f));
		ImGui::SameLine();
		if (ImGui::Button("No"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	//ImNodesStyle& style = ImNodes::GetStyle();
	ImNodes::GetStyle().Colors[ImNodesCol_GridBackground] = IM_COL32(0.05f, 0.07f, 0.120f, 1.0f);

	GUI::getSingleton()->pushMediumFont();

	ImNodes::BeginNodeEditor();

	ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;

	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();

	for (int i = 0; i < l_nodeGraph->m_nodes.size(); i++)
	{
		l_nodeGraph->m_nodes[i]->draw();
	}

	for (int i = 0; i < l_nodeGraph->m_links.size(); ++i)
	{
		const std::pair<int, int> p = l_nodeGraph->m_links[i];
		ImNodes::Link(i, p.first, p.second);
	}
	ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_TopRight);
	ImNodes::EndNodeEditor();

	ImGui::PopFont();

	int l_startAttr;
	int l_endAttr;
	if (ImNodes::IsLinkCreated(&l_startAttr, &l_endAttr))
	{
		l_nodeGraph->addLink(l_startAttr, l_endAttr);
	}

	// since we create links with identifier being its index, we can directly use it here.
	int l_linkID;
	if (ImNodes::IsLinkDestroyed(&l_linkID))
	{
		if (!l_nodeGraph->getNode(l_nodeGraph->m_links[l_linkID].second)->checkIfFinal())
		{
			l_nodeGraph->m_links.erase(l_nodeGraph->m_links.begin() + l_linkID);
			l_nodeGraph->informModification();
		}
	}
	
	const int l_numSelectedNodes = ImNodes::NumSelectedNodes();
	m_selectedNodes.resize(l_numSelectedNodes);
	if (l_numSelectedNodes > 0)
	{
		ImNodes::GetSelectedNodes(m_selectedNodes.data());
	}

	ImGui::EndChild();
}
