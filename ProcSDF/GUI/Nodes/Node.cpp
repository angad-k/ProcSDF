#include <algorithm>
#include <unordered_map>

#include "Constants/constant.h"
#include "GUI/GuiUtilities.h"
#include "GUI/Nodes/Node.h"
#include "GUI/NodeEditor.h"
#include "GUI/NodeGraph.h"
#include "GUI/Nodes/PrimitiveNodes.h"
#include "GUI/Nodes/OperationNodes.h"
#include "Utilities/logger.h"

void Node::init()
{
	NodeGraph* node_graph = NodeGraph::get_singleton();
	id = node_graph->allocate_id(this);
	variable_name = node_name + "_" + std::to_string(id);
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
	ImGui::BeginGroup();

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted(node_name.c_str());
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
	{
		ImGui::SetTooltip(variable_name.c_str());
	}
	ImNodes::EndNodeTitleBar();

	int id_counter = 0;

	assert(input_ids.size() == input_pins.size());

	for (unsigned int i = 0; i < input_pins.size(); i++)
	{
		ImNodes::BeginInputAttribute(input_ids[i]);
		ImGui::Text(input_pins[i].c_str());
		ImNodes::EndInputAttribute();
	}

	assert(output_ids.size() == output_pins.size());

	for (unsigned int i = 0; i < output_pins.size(); i++)
	{
		ImNodes::BeginOutputAttribute(output_ids[i]);
		ImGui::Text(output_pins[i].c_str());
		ImNodes::EndOutputAttribute();
	}
	
	ImGui::PushItemWidth(80);

	if (input_float3_labels.size() != 0 || input_float_labels.size() != 0)
	{
		GUI_Utilities::horizontal_seperator(15);
	}

	assert(input_float3.size() == input_float3_labels.size());

	for (unsigned int i = 0; i < input_float3_labels.size(); i++)
	{
		ImGui::Text(input_float3_labels[i].c_str());
		ImGui::PushID(id_counter++);
		ImGui::InputFloat("x", &input_float3[i][0]);
		ImGui::InputFloat("y", &input_float3[i][1]);
		ImGui::InputFloat("z", &input_float3[i][2]);
		ImGui::PopID();
		if (i != input_float3_labels.size() - 1)
		{
			GUI_Utilities::horizontal_seperator(15);
		}
	}

	if (input_float_labels.size() != 0 && input_float3_labels.size() != 0)
	{
		GUI_Utilities::horizontal_seperator(15);
	}

	assert(input_floats.size() == input_float_labels.size());

	for (unsigned int i = 0; i < input_float_labels.size(); i++)
	{
		ImGui::Text(input_float_labels[i].c_str());
		ImGui::PushID(id_counter++);
		ImGui::InputFloat("", &input_floats[i]);
		ImGui::PopID();
	}
	ImGui::PopItemWidth();

	ImGui::EndGroup();
	ImNodes::EndNode();

	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

std::string Node::get_string()
{
	std::string nodestr = shader_generation::FLOAT;
	nodestr += variable_name;
	nodestr += " = ";
	nodestr += node_name;
	nodestr += "(";
	bool comma_needed = false;
	for (unsigned int i = 0; i < input_ids.size(); i++)
	{
		Node* source_node = NodeGraph::get_singleton()->get_source_node(input_ids[i]);
		if (source_node == nullptr)
		{
			std::string compilation_error = "Input is undefined for " + variable_name;
			NodeGraph::get_singleton()->set_compilation_error(compilation_error);
			ERR(compilation_error);
			return "ERR";
		}
		std::string input_arg_name = source_node->get_variable_name();
		if (comma_needed)
		{
			nodestr += ", ";
		}
		nodestr += input_arg_name;
		comma_needed = true;
	}

	if (input_ids.size() == 0) {
		nodestr += shader_generation::POSITION;
		comma_needed = true;
	}

	for (unsigned int i = 0; i < input_float3.size(); i++)
	{
		for (unsigned int j = 0; j < input_float3[i].size(); j++)
		{
			if (comma_needed)
			{
				nodestr += ", ";
			}
			nodestr += std::to_string(input_float3[i][j]);
		}
	}

	for (unsigned int i = 0; i < input_floats.size(); i++)
	{
		if (comma_needed)
		{
			nodestr += ", ";
		}
		nodestr += std::to_string(input_floats[i]);
	}

	nodestr += ");";
	return nodestr;
}