#pragma once
#include "ImGUI/imgui.h"
#include "GUI/NodeGraph.h"
#include <string>
class Material
{
	float m_color[3] = { 1.0f, 0.0f, 0.0f };
	std::string m_name = "New Material";
	int m_ID;
public:
	bool m_is_being_edited = false;
	void draw();
	std::string get_name()
	{
		return m_name;
	}

	float* getColor()
	{
		return m_color;
	}

	int getID()
	{
		return m_ID;
	}

	std::string getColorName()
	{
		return "color_" + std::to_string(m_ID);
	}

	std::vector<std::string> getVec3Uniforms()
	{
		std::vector<std::string> l_vec3Uniforms;
		l_vec3Uniforms.push_back(getColorName());
		return l_vec3Uniforms;
	}

	void setUniforms();

	Material()
	{
		m_ID = NodeGraph::getSingleton()->allocateMaterialID(this);
		m_name = "Material#" + std::to_string(m_ID);
	}
};