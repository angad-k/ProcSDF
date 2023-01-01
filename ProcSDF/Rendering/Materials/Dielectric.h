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
	Dielectric(int p_ID) : Material(p_ID)
	{
		m_materialType = material_type::DIELECTRIC;
		m_inputFloatLabels = { "IOR", "Roughness" };
		init();
	}
};