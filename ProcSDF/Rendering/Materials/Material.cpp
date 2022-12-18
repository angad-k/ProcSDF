#include"Rendering/Materials/Material.h"
#include"Renderer.h"
#include"GUI/GuiUtilities.h"
#include"Rendering/ShaderGenerator.h"
void Material::draw()
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
			ImGui::Text(m_inputFloat3Labels[i].c_str());
			ImGui::PushID(l_idCounter++);
			ImGui::DragFloat("x", &m_inputFloat3s[i][0]);
			ImGui::DragFloat("y", &m_inputFloat3s[i][1]);
			ImGui::DragFloat("z", &m_inputFloat3s[i][2]);
			ImGui::PopID();
			Renderer::getSingleton()->setUniformFloat3(
				ShaderGenerator::getUniformStringFromLabel(m_name, m_inputFloat3Labels[i]),
				m_inputFloat3s[i][0],
				m_inputFloat3s[i][1],
				m_inputFloat3s[i][2]);
		}

		assert(m_inputFloats.size() == m_inputFloatLabels.size());
		for (unsigned int i = 0; i < m_inputFloatLabels.size(); i++)
		{
			ImGui::Text(m_inputFloatLabels[i].c_str());
			ImGui::PushID(l_idCounter++);
			ImGui::DragFloat("", &m_inputFloats[i]);
			ImGui::PopID();
			Renderer::getSingleton()->setUniformFloat(
				ShaderGenerator::getUniformStringFromLabel(m_name, m_inputFloatLabels[i]),
				m_inputFloats[i]
			);
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