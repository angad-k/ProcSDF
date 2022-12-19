#pragma once
#include "Material.h"
class Metal : public Material
{
public:
	Metal()
	{
		m_materialType = material_type::DIELECTRIC;
		m_inputFloatLabels = { "IOR" };
		init();
	}
};