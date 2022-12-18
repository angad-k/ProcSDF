#pragma once
#include "GUI/Nodes/Node.h"
#include "GUI/NodeGraph.h"

class ObjectNode : public Node {
	int m_material_id = 0;
public:
	ObjectNode()
	{
		m_variableName = "object";
		m_nodeName = "Object node";
		m_inputPins = { "Input" };
		m_outputPins = { "Final Output" };
		m_isObjectNode = true;
		m_titleColor = imgui_colors::PURPLE;
		init();
	}
	ObjectNode(int p_ID)
	{
		m_variableName = "object";
		m_nodeName = "Object node";
		m_inputPins = { "Input" };
		m_outputPins = { "Final Output" };
		m_isObjectNode = true;
		m_titleColor = imgui_colors::PURPLE;
		init(p_ID);
	}
	void draw()
	{
		drawStartNode();
		drawNodeContents();
		static ImGuiComboFlags flags = 0;
		std::vector<Material*> l_materials = NodeGraph::getSingleton()->getMaterials();
		std::string l_comboPreviewValue = l_materials[m_material_id]->get_name();
		ImGui::PushItemWidth(100);
		if (ImGui::BeginCombo("Material", l_comboPreviewValue.c_str(), flags))
		{
			for (int i = 0; i < l_materials.size(); i++)
			{
				const bool l_isSelected = (m_material_id == i);
				if (ImGui::Selectable(l_materials[i]->get_name().c_str(), l_isSelected))
					m_material_id = i;
				if (l_isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		drawEndNode();
	}
};
