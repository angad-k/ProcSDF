#include"Rendering/Materials/Material.h"
#include"Renderer.h"
#include"GUI/GuiUtilities.h"
#include"Rendering/ShaderGenerator.h"
void Material::draw(bool &p_del)
{
	if (m_is_being_edited)
	{
		char l_inputBuffer[256];
		strcpy_s(l_inputBuffer, m_name.c_str());
		if (ImGui::InputText("Name", l_inputBuffer, 256))
		{
			m_name = l_inputBuffer;
		}
		ImGui::ColorEdit3("Material color", m_color);
		Renderer::getSingleton()->setUniformFloat3(
			"color_" + std::to_string(m_ID),
			m_color[0],
			m_color[1],
			m_color[2]);

		int l_idCounter = 0;

		assert(m_inputFloat3s.size() == m_inputFloat3Labels.size());
		for (unsigned int i = 0; i < m_inputFloat3Labels.size(); i++)
		{
			ImGui::PushID(l_idCounter++);
			float l_inputFloat3[3] = { m_inputFloat3s[i][0], m_inputFloat3s[i][1], m_inputFloat3s[i][2] };
			ImGui::DragFloat3(m_inputFloat3Labels[i].c_str(), l_inputFloat3);
			ImGui::PopID();
			m_inputFloat3s[i][0] = l_inputFloat3[0];
			m_inputFloat3s[i][1] = l_inputFloat3[1];
			m_inputFloat3s[i][2] = l_inputFloat3[2];
			Renderer::getSingleton()->setUniformFloat3(
				ShaderGenerator::getUniformStringFromLabel(m_name, m_inputFloat3Labels[i]),
				m_inputFloat3s[i][0],
				m_inputFloat3s[i][1],
				m_inputFloat3s[i][2]);
		}

		assert(m_inputFloats.size() == m_inputFloatLabels.size());
		for (unsigned int i = 0; i < m_inputFloatLabels.size(); i++)
		{
			ImGui::DragFloat(m_inputFloatLabels[i].c_str(), &m_inputFloats[i], 0.01, 0.0, 1.0);
			Renderer::getSingleton()->setUniformFloat(
				ShaderGenerator::getUniformStringFromLabel(m_name, m_inputFloatLabels[i]),
				m_inputFloats[i]
			);
		}

		if (!NodeGraph::getSingleton()->canDeleteMaterial())
		{
			ImGui::BeginDisabled();
		}
		if (ImGui::Button("Delete Material"))
		{
			p_del = true;
		}
		if (!NodeGraph::getSingleton()->canDeleteMaterial())
		{
			ImGui::EndDisabled();
		}

		if (ImGui::Button("Done"))
		{
			m_is_being_edited = false;
		}
	}
	else
	{
		// std::string l_edit_mat_str = "Edit " + m_name;
		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(m_color[0] * 255, m_color[1] * 255, m_color[2] * 255, 255));
		if (!GUI_Utilities::isWhiteContrasting(m_color))
		{
			ImGui::PushStyleColor(ImGuiCol_Text, imgui_colors::BLACK);
		}
		if (ImGui::Button(m_name.c_str()))
		{
			m_is_being_edited = true;
		}
		if (!GUI_Utilities::isWhiteContrasting(m_color))
		{
			ImGui::PopStyleColor();
		}
		ImGui::PopStyleColor();
	}
	ImGui::Separator();	
}

void Material::setUniforms()
{
	Renderer::getSingleton()->setUniformFloat3(
		"color_" + std::to_string(m_ID),
		m_color[0],
		m_color[1],
		m_color[2]);

	for (unsigned int i = 0; i < m_inputFloatLabels.size(); i++)
	{
		Renderer::getSingleton()->setUniformFloat(
			ShaderGenerator::getUniformStringFromLabel(m_name, m_inputFloatLabels[i]),
			m_inputFloats[i]
		);
	}

	for (unsigned int i = 0; i < m_inputFloat3Labels.size(); i++)
	{
		Renderer::getSingleton()->setUniformFloat3(
			ShaderGenerator::getUniformStringFromLabel(m_name, m_inputFloat3Labels[i]),
			m_inputFloat3s[i][0],
			m_inputFloat3s[i][1],
			m_inputFloat3s[i][2]);
	}
}