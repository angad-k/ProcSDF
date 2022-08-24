#include "GUI/NodeEditor.h"
#include "GUI/Nodes/Node.h"
#include "GUI/NodeGraph.h"

void NodeEditor::draw()
{
	ImGui::Begin("Nodes workspace");

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

	for (auto i : nodeGraph->allocated_ids)
	{
		for (auto j : nodeGraph->allocated_ids)
		{
			int start_attr = i.first;
			int end_attr = j.first;
			if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
			{
				nodeGraph->add_link(start_attr, end_attr);
			}
		}
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