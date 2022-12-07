#include "GUI/NodeEditor.h"
#include "GUI/Nodes/Node.h"
#include "GUI/NodeGraph.h"
#include "Common/constant.h"
#include "Common/ProjectSaver.h"
#include "Common/logger.h"

void NodeEditor::draw()
{
	ImGui::Begin("Nodes workspace");
	if (ImGui::Button("Recompile"))
	{
		NodeGraph::getSingleton()->recompileNodeGraph();
	}
	
	ImGui::SameLine();

	if (ImGui::Button("Save Project"))
	{
		ProjectSaver::saveProject();
	}

	ImGui::SameLine();

	if (ImGui::Button("Load Project"))
	{
		bool isParseSucessful = ProjectSaver::loadProject();
		if (!isParseSucessful) {
			ERR("Error in parsing uploaded file");
		}
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

	ImNodes::EndNodeEditor();

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
		if (!l_nodeGraph->getNode(l_nodeGraph->m_links[l_linkID].second)->m_isFinalNode)
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

	ImGui::End();
}
