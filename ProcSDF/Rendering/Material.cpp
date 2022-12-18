#include"Rendering/Material.h"
#include"Renderer.h"
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
		std::string l_edit_mat_str = "Edit " + m_name;
		if (ImGui::Button(l_edit_mat_str.c_str()))
		{
			m_is_being_edited = true;
		}
	}
	ImGui::Separator();	
}