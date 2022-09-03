#include <algorithm>

#include "GUI/Nodes/Node.h"
#include "GUI/NodeEditor.h"
#include "GUI/NodeGraph.h"

void Node::init()
{
	NodeGraph* node_graph = NodeGraph::get_singleton();
	id = node_graph->allocate_id(this);

	input_ids = std::vector<int>(input_pins.size());
	output_ids = std::vector<int>(output_pins.size());

	std::transform(input_ids.begin(), input_ids.end(), input_ids.begin(), [node_graph, this](int i) { return node_graph->allocate_id(this); });

	std::transform(output_ids.begin(), output_ids.end(), output_ids.begin(), [node_graph, this](int i) { return node_graph->allocate_id(this); });

	input_float3 = std::vector<std::vector<float>>(input_float3_labels.size(), std::vector<float>(3, 0.0));

	input_floats = std::vector<float>(input_float_labels.size(), 0.0);

}

void Node::draw()
{
	ImNodes::PushColorStyle(
		ImNodesCol_TitleBar, title_color);

	ImNodes::PushColorStyle(
		ImNodesCol_TitleBarSelected, title_color);

	ImNodes::BeginNode(id);

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted(node_name.c_str());
	ImNodes::EndNodeTitleBar();

	for (unsigned int i = 0; i < input_pins.size(); i++)
	{
		ImNodes::BeginInputAttribute(input_ids[i]);
		ImGui::Text(input_pins[i].c_str());
		ImNodes::EndInputAttribute();
	}

	for (unsigned int i = 0; i < output_pins.size(); i++)
	{
		ImNodes::BeginOutputAttribute(output_ids[i]);
		ImGui::Text(output_pins[i].c_str());
		ImNodes::EndOutputAttribute();
	}
	
	if (input_float3_labels.size() != 0 || input_float_labels.size() != 0)
	{
		ImGui::Separator();
		ImGui::Text("Parameters");
	}

	ImGui::PushItemWidth(200);
	for (unsigned int i = 0; i < input_float3_labels.size(); i++)
	{
		float input_arr[3] = { input_float3[i][0], input_float3[i][1], input_float3[i][2] };
		ImGui::InputFloat3(input_float3_labels[i].c_str(), input_arr);
		input_float3[i][0] = input_arr[0];
		input_float3[i][1] = input_arr[1];
		input_float3[i][2] = input_arr[2];
	}
	for (unsigned int i = 0; i < input_float_labels.size(); i++)
	{
		ImGui::InputFloat(input_float_labels[i].c_str(), &input_floats[i]);
	}
	ImGui::PopItemWidth();

	ImNodes::EndNode();

	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}