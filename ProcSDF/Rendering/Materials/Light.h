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
};