#pragma once
#include "Material.h"
class Diffuse : public Material
{
public:
	Diffuse()
	{
		m_materialType = material_type::DIFFUSE;
		init();
	}
};