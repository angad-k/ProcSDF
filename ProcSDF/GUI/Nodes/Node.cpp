#include "GUI/Nodes/Node.h"
#include "GUI/NodeEditor.h"
#include "GUI/NodeGraph.h"

HelloNode::HelloNode()
{
	NodeGraph* nodeGraph = NodeGraph::get_singleton();
	id = nodeGraph->allocate_id(this);
	input_id_1 = nodeGraph->allocate_id(this);
	input_id_2 = nodeGraph->allocate_id(this);
	output_id_1 = nodeGraph->allocate_id(this);
	output_id_2 = nodeGraph->allocate_id(this);
}

void HelloNode::draw()
{
	ImNodes::BeginNode(id);

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Hello Node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(input_id_1);
	ImGui::Button("Hellu");
	ImGui::Text("input pin");
	ImNodes::EndInputAttribute();

	const int output_attr_id = 2;
	ImNodes::BeginOutputAttribute(output_id_1);
	ImGui::Button("Hellu");
	ImGui::Text("output pin 1");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginOutputAttribute(output_id_2);
	ImGui::Button("Hellu");
	ImGui::Text("output pin 2");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
}

FinalNode::FinalNode()
{
	is_final_node = true;
	NodeGraph* node_graph = NodeGraph::get_singleton();
	id = node_graph->allocate_id(this);
	input_id = node_graph->allocate_id(this);
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

SphereNode::SphereNode()
{
	is_final_node = true;
	NodeGraph* node_graph = NodeGraph::get_singleton();
	id = node_graph->allocate_id(this);
	output_id = node_graph->allocate_id(this);
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


