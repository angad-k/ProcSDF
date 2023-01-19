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
	virtual void draw(bool &p_del);
	std::string get_name()
	{
		return m_name;
	}

	float* getColor()
	{
		return m_color;
	}

	int getFloatLabelsSize() 
	{
		return m_inputFloatLabels.size();
	}

	int getFloat3LabelsSize()
	{
		return m_inputFloat3Labels.size();
	}

	void setColor(float* p_color)
	{
		m_color[0] = p_color[0];
		m_color[1] = p_color[1];
		m_color[2] = p_color[2];
	}

	void setInputFloats(std::vector<float> p_inputFloats) 
	{
		m_inputFloats = p_inputFloats;
	}

	void setInputFloat3s(std::vector<std::vector<float>> p_inputFloat3s)
	{
		m_inputFloat3s = p_inputFloat3s;
	}

	int getID()
	{
		return m_ID;
	}

	std::string getColorName()
	{
		return "color_" + std::to_string(m_ID);
	}

	std::vector<float> getInputFloats()
	{
		return m_inputFloats;
	}

	std::vector<std::vector<float>> getInputFloat3s()
	{
		return m_inputFloat3s;
	}

	std::vector<std::string> getVec3Uniforms(bool p_includeColors = true)
	{
		std::vector<std::string> l_vec3Uniforms;
		if(p_includeColors)
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

	std::string get_uniform_label(std::string p_property)
	{
		return ShaderGenerator::getUniformStringFromLabel(m_name, p_property);
	}

	void setUniforms();

	Material()
	{
		m_ID = NodeGraph::getSingleton()->allocateMaterialID(this);
		m_name = "Material#" + std::to_string(m_ID);
	}

	Material(int p_ID) 
	{
		m_ID = p_ID;
		m_name = "Material#" + std::to_string(m_ID);
		NodeGraph::getSingleton()->setMaterialID(this, p_ID);
	}

	~Material()
	{
		NodeGraph::getSingleton()->deallocateMaterialID(m_ID);
	}

	std::string get_params_string(bool p_includeColor = false)
	{
		std::string l_params_string = "";
		std::string l_comma = ", ";
		std::vector<std::string>l_floatUniforms = getFloatUniforms();
		std::vector<std::string>l_vec3uniforms = getVec3Uniforms(p_includeColor);
		for (int i = 0; i < l_vec3uniforms.size(); i++)
		{
			l_params_string.append(l_vec3uniforms[i]);
			if (i != l_vec3uniforms.size() - 1)
			{
				l_params_string.append(l_comma);
			}
		}
		if (l_floatUniforms.size() != 0 && l_vec3uniforms.size() != 0)
		{
			l_params_string.append(l_comma);
		}
		for (int i = 0; i < l_floatUniforms.size(); i++)
		{
			l_params_string.append(l_floatUniforms[i]);
			if (i != l_floatUniforms.size() - 1)
			{
				l_params_string.append(l_comma);
			}
		}
		return l_params_string;
	}

	void init()
	{
		m_inputFloat3s = std::vector<std::vector<float>>(m_inputFloat3Labels.size(), std::vector<float>(3, 0.0));
		m_inputFloats = std::vector<float>(m_inputFloatLabels.size(), 0.0);
	}
};