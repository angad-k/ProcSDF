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
	Diffuse(int p_ID) : Material(p_ID)
	{
		m_materialType = material_type::DIFFUSE;
		init();
	}
};