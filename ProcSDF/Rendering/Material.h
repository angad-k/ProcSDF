#pragma once
#include "ImGUI/imgui.h"
#include <string>
class Material
{
	float m_color[3] = { 1.0f, 0.0f, 0.0f };
	std::string m_name = "New Material";
public:
	bool m_is_being_edited = false;
	void draw();
	std::string get_name()
	{
		return m_name;
	}
};