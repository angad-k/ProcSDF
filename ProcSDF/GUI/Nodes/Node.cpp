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
#include "Rendering/ShaderGenerator.h"

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

Node::~Node()
{
	NodeGraph* node_graph = NodeGraph::get_singleton();
	node_graph->deallocate_id(id);

	for (int input_id : input_ids)
	{
		node_graph->deallocate_id(input_id);
	}
	node_graph->remove_link_with_endpoints(input_ids);

	for (int output_id : output_ids)
	{
		node_graph->deallocate_id(output_id);
	}
	node_graph->remove_link_with_endpoints(output_ids);
}

void Node::draw()
{
	ImNodes::PushColorStyle(
		ImNodesCol_TitleBar, title_color);

	//ImNodes::PushColorStyle(
	//	ImNodesCol_TitleBarSelected, title_color);

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
		Renderer::get_singleton()->set_uniform_float3(
			ShaderGenerator::get_uniform_string_from_label(variable_name, input_float3_labels[i]),
			input_float3[i][0],
			input_float3[i][1],
			input_float3[i][2]);
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
		Renderer::get_singleton()->set_uniform_float(
			ShaderGenerator::get_uniform_string_from_label(variable_name, input_float_labels[i]),
			input_floats[i]
		);
	}
	ImGui::PopItemWidth();

	ImGui::EndGroup();
	ImNodes::EndNode();

	//ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

std::string Node::get_string()
{
	std::string nodestr = shader_generation::FLOAT;
	nodestr.append(variable_name);
	nodestr.append(" = ");
	nodestr.append(node_name);
	nodestr.append("(");
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

		std::pair<Node*, int> non_transform_node_info = previous_non_transform_node_info[i];
		std::string input_arg_name = non_transform_node_info.first->get_variable_name();
		input_arg_name.append("_");
		input_arg_name.append(std::to_string(non_transform_node_info.second));
		if (comma_needed)
		{
			nodestr.append(", ");
		}
		nodestr.append(input_arg_name);
		comma_needed = true;
	}

	if (input_ids.size() == 0) {
		nodestr.append(shader_generation::POSITION);
		nodestr.append("_");
		nodestr.append(std::to_string(Node::id));
		comma_needed = true;
	}

	for (std::string float3_label : input_float3_labels)
	{
		if (comma_needed)
		{
			nodestr.append(", ");
		}
		std::string uniform_label = ShaderGenerator::get_uniform_string_from_label(variable_name, float3_label);
		nodestr.append(uniform_label + ".x");
		nodestr.append(", ");
		nodestr.append(uniform_label + ".y");
		nodestr.append(", ");
		nodestr.append(uniform_label + ".z");
	}

	for (std::string float_label : input_float_labels)
	{
		if (comma_needed)
		{
			nodestr.append(", ");
		}
		std::string uniform_label = ShaderGenerator::get_uniform_string_from_label(variable_name, float_label);
		nodestr.append(uniform_label);
	}

	nodestr.append(");");
	return nodestr;
}
