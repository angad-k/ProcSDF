#pragma once
#include "Material.h"
#include "Common/custom.h"
#include "Common/logger.h"
#include "Common/os.h"
class CustomMaterial : public Material {
	bool m_isMalformed = false;
	std::string m_CustomName;
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
	static void AddCustomMaterialAtFilePath(std::string p_filePath);
};