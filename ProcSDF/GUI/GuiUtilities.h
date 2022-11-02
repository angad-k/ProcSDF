#pragma once
#include "ImGui/imgui.h"
#include <string>

namespace GUI_Utilities {
	static void horizontalSeperator(unsigned int p_width)
	{
		// since we are using underscore, we move the cursor a 
		// little up so that the distance doesn't seem weird.
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
		ImGui::Text(std::string(p_width, '_').c_str());
	}

	static void appendToSameLineIfApplicable(float p_spaceNeeded)
	{
		ImGui::SameLine();
		if (ImGui::GetContentRegionAvail().x <= p_spaceNeeded)
		{
			ImGui::NewLine();
		}
	}
}
