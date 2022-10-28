#include "GUI/NodeEditor.h"
#include "GUI/Nodes/Node.h"
#include "GUI/NodeGraph.h"
#include "Constants/constant.h"

void NodeEditor::draw()
{
	ImGui::Begin("Nodes workspace");
	if (ImGui::Button("Recompile"))
	{
		NodeGraph::get_singleton()->recompile_node_graph();
	}
	ImGui::SameLine();

	if (selected_nodes.size() == 0)
	{
		ImGui::BeginDisabled();
	}
	ImGui::PushStyleColor(ImGuiCol_Button, imgui_colors::RED);
	if (ImGui::Button("Delete selected nodes"))
	{
		for (int sel_id : selected_nodes)
		{
			NodeGraph::get_singleton()->delete_node(sel_id);
		}
	}
	ImGui::PopStyleColor();
	ImGui::SameLine();
	if (selected_nodes.size() == 0)
	{
		ImGui::EndDisabled();
	}
	selected_nodes.clear();

	ImGui::PushStyleColor(ImGuiCol_Text, imgui_colors::RED);
	ImGui::Text(NodeGraph::get_singleton()->get_compilation_error().c_str());
	ImGui::PopStyleColor();
	ImGui::PushStyleColor(ImGuiCol_Text, imgui_colors::ORANGE);
	if ((!NodeGraph::get_singleton()->check_compilation_error()) && NodeGraph::get_singleton()->is_dirty())
	{
		ImGui::SameLine();
		ImGui::Text("Node Graph modified, recompile for changes to take effect.");
	}
	ImGui::PopStyleColor();
	// ImNodes workspace starts from here.

	ImNodes::BeginNodeEditor();

	ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;

	NodeGraph* nodeGraph = NodeGraph::get_singleton();

	for (int i = 0; i < nodeGraph->nodes.size(); i++)
	{
		nodeGraph->nodes[i]->draw();
	}

	for (int i = 0; i < nodeGraph->links.size(); ++i)
	{
		const std::pair<int, int> p = nodeGraph->links[i];
		ImNodes::Link(i, p.first, p.second);
	}

	ImNodes::EndNodeEditor();

	int start_attr;
	int end_attr;
	if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
	{
		nodeGraph->add_link(start_attr, end_attr);
	}

	// since we create links with identifier being its index, we can directly use it here.
	int link_id;
	if (ImNodes::IsLinkDestroyed(&link_id))
	{
		if (!nodeGraph->get_node(nodeGraph->links[link_id].second)->is_final_node)
		{
			nodeGraph->links.erase(nodeGraph->links.begin() + link_id);
			nodeGraph->inform_modification();
		}
	}
	
	const int num_selected_nodes = ImNodes::NumSelectedNodes();
	selected_nodes.resize(num_selected_nodes);
	if (num_selected_nodes > 0)
	{
		ImNodes::GetSelectedNodes(selected_nodes.data());
	}

	ImGui::End();
}
