#pragma once
#include "Material.h"
class Dielectric : public Material
{
public:
	Dielectric()
	{
		m_materialType = material_type::DIELECTRIC;
		m_inputFloatLabels = { "IOR", "Roughness" };
		init();
	}
};