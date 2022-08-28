#include "GUI/Nodes/Node.h"
#include "GUI/NodeEditor.h"
#include "GUI/NodeGraph.h"

FinalNode::FinalNode()
{
	is_final_node = true;
	NodeGraph* node_graph = NodeGraph::get_singleton();
	id = node_graph->allocate_id(this);
	input_id = node_graph->allocate_id(this);
	node_type = NodeTypes::FINAL_NODE;
}

void FinalNode::draw()
{
	ImNodes::BeginNode(id);

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Final Node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(input_id);
	ImGui::Text("Final Output");
	ImNodes::EndInputAttribute();

	ImNodes::EndNode();
}