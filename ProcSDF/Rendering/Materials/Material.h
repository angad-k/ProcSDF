#pragma once
#include "ImGUI/imgui.h"
#include "GUI/NodeGraph.h"
#include "Rendering/ShaderGenerator.h"
#include <string>
class Material
{
	float m_color[3] = { 1.0f, 0.0f, 0.0f };
	std::string m_name = "New Material";
	int m_ID;
protected:
	std::vector<std::string>m_inputFloatLabels;
	std::vector<float>m_inputFloats;
	std::vector<std::string>m_inputFloat3Labels;
	std::vector<std::vector<float>>m_inputFloat3s;
public:
	std::string m_materialType;
	bool m_is_being_edited = false;
	virtual void draw();
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
		for (std::string label : m_inputFloat3Labels)
		{
			l_vec3Uniforms.push_back(ShaderGenerator::getUniformStringFromLabel(m_name, label));
		}
		return l_vec3Uniforms;
	}

	std::vector<std::string> getFloatUniforms()
	{
		std::vector<std::string> l_floatUniforms;
		for (std::string label : m_inputFloatLabels)
		{
			l_floatUniforms.push_back(ShaderGenerator::getUniformStringFromLabel(m_name, label));
		}
		return l_floatUniforms;
	}

	void setUniforms();

	Material()
	{
		m_ID = NodeGraph::getSingleton()->allocateMaterialID(this);
		m_name = "Material#" + std::to_string(m_ID);
	}

	void init()
	{
		m_inputFloat3s = std::vector<std::vector<float>>(m_inputFloat3Labels.size(), std::vector<float>(3, 0.0));
		m_inputFloats = std::vector<float>(m_inputFloatLabels.size(), 0.0);
	}
};