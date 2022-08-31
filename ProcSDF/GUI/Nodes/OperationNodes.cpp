#include "GUI/Nodes/OperationNodes.h"
#include "GUI/NodeEditor.h"
#include "GUI/NodeGraph.h"
#include "GUI/Nodes/Node.h"

IntersectionNode::IntersectionNode()
{
	is_final_node = true;
	NodeGraph* node_graph = NodeGraph::get_singleton();
	id = node_graph->allocate_id(this);
	output_id = node_graph->allocate_id(this);
	input_id_1 = node_graph->allocate_id(this);
	input_id_2 = node_graph->allocate_id(this);

	node_type = NodeTypes::INTERSECTION_OPERATION;
}

void IntersectionNode::draw()
{
	ImNodes::PushColorStyle(
		ImNodesCol_TitleBar, IM_COL32(255, 50, 50, 255));

	ImNodes::PushColorStyle(
		ImNodesCol_TitleBarSelected, IM_COL32(255, 50, 50, 255));

	ImNodes::BeginNode(id);

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Intersection");
	ImNodes::EndNodeTitleBar();
	
	ImNodes::BeginInputAttribute(input_id_1);
	ImNodes::EndInputAttribute();

	ImNodes::BeginOutputAttribute(output_id);
	ImNodes::EndOutputAttribute();

	
	ImNodes::BeginInputAttribute(input_id_2);
	ImNodes::EndInputAttribute();


	ImNodes::EndNode();

	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}