#pragma once
#include "ImGui/imgui.h"
#include <string>

namespace GUI_Utilities {
	static void horizontalSeperator(unsigned int p_width)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
		ImGui::Text(std::string(p_width, '_').c_str());
	}
}
