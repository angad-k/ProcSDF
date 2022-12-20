#pragma once
#include<string>
#include<vector>
#include "GUI/NodeGraph.h"
#include "GUI/Nodes/TransformNodes.h"
class ShaderGenerator
{
private:
	std::string m_shader;
	int m_objectCount;
	static ShaderGenerator* s_shaderGenerator;
	std::map<int, int> m_nodeIDToObjectIDMap;
	
	std::vector<std::string> m_uniformVec3;
	std::vector<std::string> m_uniformFloats;
	std::map<int, int> m_objectIDToNodeIDMap;

	std::string generateVec3StringFromFloatArray(float color[]);
	
	bool m_shaderModified = true;

	std::string generateShaderFilePath(std::string p_fileName);
	std::string generateUniformDeclarations();
	std::string generateObjectFunctions();
	std::string generateGetColorFunction();
	std::string generateClosestObjectInfoFunction();
	std::string generateCalculateNormalFunction();
	std::string generateGetTargetRayFunction();
	std::string generateGetDistanceFromFunction();

	std::string getTransform(TransformNode* p_node);
	void computeAndSetObjectCount();
	void computeUniforms();
	void appendCustomFunctions(std::string& p_shaderString);
public:
	static ShaderGenerator* getSingleton() {
		if (!s_shaderGenerator)
		{
			s_shaderGenerator = new ShaderGenerator();
		}
		return s_shaderGenerator;
	}

	static std::string getUniformStringFromLabel(std::string p_variable_name, std::string p_label);

	std::string getShader() {
		return m_shader;
	}

	bool isShaderModified()
	{
		return m_shaderModified;
	}

	void setShaderModificationHandled()
	{
		m_shaderModified = false;
	}

	void setShader(std::string p_shader) {
		this->m_shader = p_shader;
	}

	int getObjectCount() {
		return m_objectCount;
	}

	void setObjectCount(int p_count) {
		m_objectCount = p_count;
	}

	void generateAndSetShader();
};