#include "GUI/Nodes/Node.h"
#include "GUI/NodeEditor.h"
#include "GUI/NodeGraph.h"

void Node::init()
{
	NodeGraph* node_graph = NodeGraph::get_singleton();
	id = node_graph->allocate_id(this);
	for (unsigned int i = 0; i < input_pins.size(); i++)
	{
		input_ids.push_back(node_graph->allocate_id(this));
	}

	for (unsigned int i = 0; i < output_pins.size(); i++)
	{
		output_ids.push_back(node_graph->allocate_id(this));
	}
}

void Node::draw()
{
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

	ImNodes::EndNode();
}