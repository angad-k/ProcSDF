#pragma once
#include "Material.h"
class Metal : public Material
{
public :
	Metal()
	{
		m_inputFloatLabels = { "Roughness" };
		m_inputFloat3Labels = { "Test" };
		init();
	}
};