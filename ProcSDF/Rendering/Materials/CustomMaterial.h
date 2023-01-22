#pragma once
#include "Material.h"
#include "Common/custom.h"
#include "Common/logger.h"
#include "Common/os.h"
class CustomMaterial : public Material {
	bool m_isMalformed = false;
	std::string m_CustomName;
	void parseCustomMaterialFile(std::string p_materialName);
public:
	bool isMalformed()
	{
		return m_isMalformed;
	}
	std::string getCustomName()
	{
		return m_CustomName;
	}
	CustomMaterial(std::string p_materialName);
	CustomMaterial(std::string p_materialName, int p_ID);
	static void AddCustomMaterialAtFilePath(std::string p_filePath);
	static void AddCustomMaterialWithFileContent(std::string p_fileContent);
};