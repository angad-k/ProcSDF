#include"Rendering/Material.h"
#include"Renderer.h"
#include"GUI/GuiUtilities.h"
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
}