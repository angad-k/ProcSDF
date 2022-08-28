#include "GUI/Nodes/PrimitiveNodes.h"
#include "GUI/NodeEditor.h"
#include "GUI/NodeGraph.h"
#include "GUI/Nodes/Node.h"

SphereNode::SphereNode()
{
	is_final_node = true;
	NodeGraph* node_graph = NodeGraph::get_singleton();
	id = node_graph->allocate_id(this);
	output_id = node_graph->allocate_id(this);
	node_type = NodeTypes::SPHERE_PRIMITIVE;
}

void SphereNode::draw()
{
	ImNodes::PushColorStyle(
		ImNodesCol_TitleBar, IM_COL32(255, 50, 50, 255));

	ImNodes::PushColorStyle(
		ImNodesCol_TitleBarSelected, IM_COL32(255, 50, 50, 255));

	ImNodes::BeginNode(id);

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Sphere");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginOutputAttribute(output_id);
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();

	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}