#include "GUI/NodeEditor.h"
#include "GUI/Nodes/Node.h"
#include "GUI/NodeGraph.h"

void NodeEditor::draw()
{
	ImGui::Begin("Nodes workspace");

	ImNodes::BeginNodeEditor();

	ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;

	NodeGraph* nodeGraph = NodeGraph::get_singleton();

	for (auto i : nodeGraph->adjacency_list){
		nodeGraph->allocated_ids[i.first]->draw();
	}

	for (auto i : nodeGraph->adjacency_list){
		for (int j : i.second) {
		
			ImNodes::Link(NodeGraph::get_hash(i.first, j), i.first, j);
		}
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
	for (auto i : nodeGraph->adjacency_list)
	{
		for (int j : i.second) {
			
			int edge_hash = NodeGraph::get_hash(i.first, j);

			if (ImNodes::IsLinkDestroyed(&edge_hash)) {
				nodeGraph->adjacency_list[i.first].erase(j);
				if (nodeGraph->adjacency_list[i.first].size() == 0) {
					nodeGraph->adjacency_list.erase(i.first);
				}
			}

		}
	}

	ImGui::End();
}