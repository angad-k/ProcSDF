#include <algorithm>
#include <unordered_map>

#include "Common/constant.h"
#include "GUI/GuiUtilities.h"
#include "GUI/Nodes/Node.h"
#include "GUI/NodeEditor.h"
#include "GUI/NodeGraph.h"
#include "GUI/Nodes/PrimitiveNodes.h"
#include "GUI/Nodes/OperationNodes.h"
#include "Common/logger.h"
#include "Rendering/ShaderGenerator.h"

void Node::init()
{
	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
	m_ID = l_nodeGraph->allocateID(this);
	m_variableName = m_nodeName + "_" + std::to_string(m_ID);
	m_inputIDs = std::vector<int>(m_inputPins.size());
	m_outputIDs = std::vector<int>(m_outputPins.size());

	std::transform(m_inputIDs.begin(), m_inputIDs.end(), m_inputIDs.begin(), [l_nodeGraph, this](int i) { return l_nodeGraph->allocateID(this); });

	std::transform(m_outputIDs.begin(), m_outputIDs.end(), m_outputIDs.begin(), [l_nodeGraph, this](int i) { return l_nodeGraph->allocateID(this); });

	m_inputFloat3 = std::vector<std::vector<float>>(m_inputFloat3Labels.size(), std::vector<float>(3, 0.0));

	m_inputFloats = std::vector<float>(m_inputFloatLabels.size(), 0.0);

}

Node::~Node()
{
	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
	l_nodeGraph->deallocateID(m_ID);

	for (int l_inputID : m_inputIDs)
	{
		l_nodeGraph->deallocateID(l_inputID);
	}
	l_nodeGraph->removeLinkWithEndpoints(m_inputIDs);

	for (int l_outputID : m_outputIDs)
	{
		l_nodeGraph->deallocateID(l_outputID);
	}
	l_nodeGraph->removeLinkWithEndpoints(m_outputIDs);
}

void Node::draw()
{
	ImNodes::PushColorStyle(
		ImNodesCol_TitleBar, m_titleColor);

	//ImNodes::PushColorStyle(
	//	ImNodesCol_TitleBarSelected, title_color);

	ImNodes::BeginNode(m_ID);
	ImGui::BeginGroup();

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted(m_nodeName.c_str());
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
	{
		ImGui::SetTooltip(m_variableName.c_str());
	}
	ImNodes::EndNodeTitleBar();

	int l_idCounter = 0;

	assert(m_inputIDs.size() == m_inputPins.size());

	for (unsigned int i = 0; i < m_inputPins.size(); i++)
	{
		ImNodes::BeginInputAttribute(m_inputIDs[i]);
		ImGui::Text(m_inputPins[i].c_str());
		ImNodes::EndInputAttribute();
	}

	assert(m_outputIDs.size() == m_outputPins.size());

	for (unsigned int i = 0; i < m_outputPins.size(); i++)
	{
		ImNodes::BeginOutputAttribute(m_outputIDs[i]);
		ImGui::Text(m_outputPins[i].c_str());
		ImNodes::EndOutputAttribute();
	}
	
	ImGui::PushItemWidth(80);

	if (m_inputFloat3Labels.size() != 0 || m_inputFloatLabels.size() != 0)
	{
		GUI_Utilities::horizontalSeperator(15);
	}

	assert(m_inputFloat3.size() == m_inputFloat3Labels.size());

	for (unsigned int i = 0; i < m_inputFloat3Labels.size(); i++)
	{
		ImGui::Text(m_inputFloat3Labels[i].c_str());
		ImGui::PushID(l_idCounter++);
		ImGui::DragFloat("x", &m_inputFloat3[i][0]);
		ImGui::DragFloat("y", &m_inputFloat3[i][1]);
		ImGui::DragFloat("z", &m_inputFloat3[i][2]);
		ImGui::PopID();
		if (i != m_inputFloat3Labels.size() - 1)
		{
			GUI_Utilities::horizontalSeperator(15);
		}
		Renderer::getSingleton()->setUniformFloat3(
			ShaderGenerator::getUniformStringFromLabel(m_variableName, m_inputFloat3Labels[i]),
			m_inputFloat3[i][0],
			m_inputFloat3[i][1],
			m_inputFloat3[i][2]);
	}

	if (m_inputFloatLabels.size() != 0 && m_inputFloat3Labels.size() != 0)
	{
		GUI_Utilities::horizontalSeperator(15);
	}

	assert(m_inputFloats.size() == m_inputFloatLabels.size());

	for (unsigned int i = 0; i < m_inputFloatLabels.size(); i++)
	{
		ImGui::Text(m_inputFloatLabels[i].c_str());
		ImGui::PushID(l_idCounter++);
		ImGui::DragFloat("", &m_inputFloats[i]);
		ImGui::PopID();
		Renderer::getSingleton()->setUniformFloat(
			ShaderGenerator::getUniformStringFromLabel(m_variableName, m_inputFloatLabels[i]),
			m_inputFloats[i]
		);
	}
	ImGui::PopItemWidth();

	ImGui::EndGroup();
	ImNodes::EndNode();

	//ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

std::string Node::m_getString()
{
	std::string l_nodestr = shader_generation::FLOAT;
	l_nodestr.append(m_variableName);
	l_nodestr.append(" = ");
	l_nodestr.append(m_nodeName);
	l_nodestr.append("(");
	bool l_commaNeeded = false;
	for (unsigned int i = 0; i < m_inputIDs.size(); i++)
	{
		Node* l_sourceNode = NodeGraph::getSingleton()->getSourceNode(m_inputIDs[i]);
		if (l_sourceNode == nullptr)
		{
			std::string l_compilationError = "Input is undefined for " + m_variableName;
			NodeGraph::getSingleton()->setCompilationError(l_compilationError);
			ERR(l_compilationError);
			return "ERR";
		}

		std::string l_inputArgName = m_previousNonTransformNode[i]->getVariableName() + std::to_string(i);
		if (l_commaNeeded)
		{
			l_nodestr.append(", ");
		}
		l_nodestr.append(l_inputArgName);
		l_commaNeeded = true;
	}

	if (m_inputIDs.size() == 0) {
		l_nodestr.append(shader_generation::POSITION);
		l_commaNeeded = true;
	}

	for (std::string l_float3Label : m_inputFloat3Labels)
	{
		if (l_commaNeeded)
		{
			l_nodestr.append(", ");
		}
		std::string l_uniformLabel = ShaderGenerator::getUniformStringFromLabel(m_variableName, l_float3Label);
		l_nodestr.append(l_uniformLabel + ".x");
		l_nodestr.append(", ");
		l_nodestr.append(l_uniformLabel + ".y");
		l_nodestr.append(", ");
		l_nodestr.append(l_uniformLabel + ".z");
	}

	for (std::string l_floatLabel : m_inputFloatLabels)
	{
		if (l_commaNeeded)
		{
			l_nodestr.append(", ");
		}
		std::string l_uniformLabel = ShaderGenerator::getUniformStringFromLabel(m_variableName, l_floatLabel);
		l_nodestr.append(l_uniformLabel);
	}

	l_nodestr.append(");");
	return l_nodestr;
}
