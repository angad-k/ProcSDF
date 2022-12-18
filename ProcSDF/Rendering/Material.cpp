#include"Rendering/Material.h"

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
		if (ImGui::Button("Done"))
		{
			m_is_being_edited = false;
		}
	}
	else
	{
		std::string l_edit_mat_str = "Edit " + m_name;
		if (ImGui::Button(l_edit_mat_str.c_str()))
		{
			m_is_being_edited = true;
		}
	}
	ImGui::Separator();	
}