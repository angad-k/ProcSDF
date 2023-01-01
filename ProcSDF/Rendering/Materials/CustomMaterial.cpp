#include "CustomMaterial.h"

CustomMaterial::CustomMaterial(std::string p_materialName) : Material()
{
	m_materialType = material_type::CUSTOM;
	CustomMaterial::parseCustomMaterialFile(p_materialName);
	init();
}

CustomMaterial::CustomMaterial(std::string p_materialName, int p_ID) : Material(p_ID)
{
	m_materialType = material_type::CUSTOM;
	CustomMaterial::parseCustomMaterialFile(p_materialName);
	init();
}

void CustomMaterial::parseCustomMaterialFile(std::string p_materialName) {

	std::string l_fileContent = NodeGraph::getSingleton()->getCustomMaterialFileContentsfromMaterialName(p_materialName);
	std::vector<std::string> l_lines = custom::tokenizeWithDelimiters(l_fileContent, "\n");

	for (std::string line : l_lines)
	{
		std::vector<std::string> l_tokens = custom::tokenizeWithDelimiters(line, "\\s+");

		if (l_tokens[0] == "//")
		{
			if (l_tokens[1] == "name")
			{
				if (l_tokens.size() <= 2)
				{
					ERR("type should be followed by an argument.");
					m_isMalformed = true;
					break;
				}
				m_CustomName = l_tokens[2];
			}
			else if (l_tokens[1] == "vec3_params")
			{
				m_inputFloat3Labels.insert(m_inputFloat3Labels.end(), l_tokens.begin() + 2, l_tokens.end());
			}
			else if (l_tokens[1] == "float_params")
			{
				m_inputFloatLabels.insert(m_inputFloatLabels.end(), l_tokens.begin() + 2, l_tokens.end());
			}
		}
	}
}

void CustomMaterial::AddCustomMaterialAtFilePath(std::string p_filePath)
{
	std::string l_fileContent = OS::fetchFileContent(p_filePath);
	std::vector<std::string> l_lines = custom::tokenizeWithDelimiters(l_fileContent, "\n");
	bool l_isMalformed = false;
	std::string l_materialName = "";
	for (std::string line : l_lines)
	{
		std::vector<std::string> l_tokens = custom::tokenizeWithDelimiters(line, "\\s+");

		if (l_tokens[0] == "//")
		{
			if (l_tokens[1] == "name")
			{
				if (l_tokens.size() <= 2)
				{
					ERR("type should be followed by an argument.");
					l_isMalformed = true;
					break;
				}
				l_materialName = l_tokens[2];
			}
		}
	}
	if (!l_isMalformed)
	{
		NodeGraph::getSingleton()->setCustomMaterialFileContents(l_materialName, l_fileContent);
		NodeGraph::getSingleton()->setCustomMaterialFilePath(l_materialName, p_filePath);
	}
}
