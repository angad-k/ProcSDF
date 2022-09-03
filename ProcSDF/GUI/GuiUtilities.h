#pragma once
#include "ImGui/imgui.h"
#include <string>

namespace GUI_Utilities {
	static void horizontal_seperator(unsigned int width)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
		ImGui::Text(std::string(width, '_').c_str());
	}
}
