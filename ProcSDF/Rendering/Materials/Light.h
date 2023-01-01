#pragma once
#include "Material.h"
class Light : public Material
{
public:
	Light()
	{
		m_materialType = material_type::LIGHT;
		m_inputFloatLabels = { "Energy" };
		init();
	}

	Light(int p_ID) : Material(p_ID)
	{
		m_materialType = material_type::LIGHT;
		m_inputFloatLabels = { "Energy" };
		init();
	}
};