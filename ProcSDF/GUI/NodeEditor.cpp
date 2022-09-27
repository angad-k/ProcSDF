#include "GUI/NodeEditor.h"
#include "GUI/Nodes/Node.h"
#include "GUI/NodeGraph.h"

void NodeEditor::draw()
{
	ImGui::Begin("Nodes workspace");
	if (ImGui::Button("Recompile"))
	{
		NodeGraph::get_singleton()->recompile_node_graph();
	}
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
	ImGui::Text(NodeGraph::get_singleton()->get_compilation_error().c_str());
	ImGui::PopStyleColor();
	// ImNodes workspace starts from here.

	ImNodes::BeginNodeEditor();

	ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;

	NodeGraph* nodeGraph = NodeGraph::get_singleton();

	std::cout << "add node in\n";
	for (int i = 0; i < nodeGraph->nodes.size(); i++)
	{
		std::cout << nodeGraph->nodes[i]->id << " ";
		nodeGraph->nodes[i]->draw();
	}
	std::cout << "\n";

	
	for (int i = 0; i < nodeGraph->links.size(); ++i)
	{
		std::cout << "add link in\n";
		const std::pair<int, int> p = nodeGraph->links[i];
		std::cout << p.first << " " << p.second << "\n";
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
	for (int i = 0; i < nodeGraph->links.size(); i++)
	{
		if (ImNodes::IsLinkDestroyed(&i))
		{
			nodeGraph->links.erase(nodeGraph->links.begin() + i);
		}
	}


	ImGui::End();
}