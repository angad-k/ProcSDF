#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include "Rendering/Renderer.h"
#include "Rendering/ShaderGenerator.h"
#include "GUI/Nodes/ObjectNode.h"
#include "GUI/Nodes/OperationNodes.h"
#include "Common/constant.h"
#include "GUI/NodeGraph.h"
#include "Common/os.h"
#include "Rendering/Materials/CustomMaterial.h"

void ShaderGenerator::computeAndSetObjectCount() {
	
	int l_objectCount = 0;
	for (Node* node : NodeGraph::getSingleton()->m_nodes) {
		if (node->checkIfObject()) {
			l_objectCount++;
			ShaderGenerator::m_nodeIDToObjectIDMap[node->m_ID] = l_objectCount;
			ShaderGenerator::m_objectIDToNodeIDMap[l_objectCount] = node->m_ID;
		}
	}

	ShaderGenerator::setObjectCount(l_objectCount);
}

void ShaderGenerator::computeUniforms()
{
	m_uniformVec3.clear();
	m_uniformFloats.clear();
	for (Node* node : NodeGraph::getSingleton()->m_nodes) {
		std::string l_variableName = node->getVariableName();
		for (std::string l_vec3Label : node->m_inputFloat3Labels)
		{
			m_uniformVec3.push_back(getUniformStringFromLabel(l_variableName, l_vec3Label));
		}
		for (std::string l_floatLabel : node->m_inputFloatLabels)
		{
			m_uniformFloats.push_back(getUniformStringFromLabel(l_variableName, l_floatLabel));
		}
	}
	for (Material* material : NodeGraph::getSingleton()->getMaterials())
	{
		for (std::string label : material->getVec3Uniforms())
		{
			m_uniformVec3.push_back(label);
		}

		for (std::string label : material->getFloatUniforms())
		{
			m_uniformFloats.push_back(label);
		}
	}
}

void ShaderGenerator::appendCustomFunctions(std::string &p_shaderString)
{
	std::vector<std::string> l_customNodeFileContents = NodeGraph::getSingleton()->getCustomNodeFileContents();
	for (std::string l_fileContent : l_customNodeFileContents)
	{
		p_shaderString.append(l_fileContent);
	}

	std::vector<std::string> l_customMaterialFileContents = NodeGraph::getSingleton()->getCustomMaterialFileContents();
	for (std::string l_fileContent : l_customMaterialFileContents)
	{
		p_shaderString.append(l_fileContent);
	}
}

std::string ShaderGenerator::generateVec3StringFromFloatArray(float color[])
{
	std::string l_vec3 = shader_generation::VEC3;
	for (int i = 0; i < 3; i++)
	{
		l_vec3.replace(l_vec3.find('$'), 1, std::to_string(color[i]));
	}
	return l_vec3;
}

std::string ShaderGenerator::generateGetColorFunction()
{
	std::string l_getColorFunction = shader_generation::get_color::FUNCTION_TEMPLATE;

	std::string l_switchStatement = shader_generation::SWITCH_STATEMENT;

	std::string l_getColorCases = "";

	for (Node* node : NodeGraph::getSingleton()->m_nodes) {
		if (node->checkIfObject()) {
			ObjectNode* l_node = (ObjectNode*)node;
			std::string l_caseStatement = shader_generation::get_color::CASE_STATEMENT;
			l_caseStatement.replace(l_caseStatement.find('$'), 1, std::to_string(ShaderGenerator::m_nodeIDToObjectIDMap[node->m_ID]));
			l_caseStatement.replace
			(
				l_caseStatement.find('$'),
				1,
				NodeGraph::getSingleton()->getMaterialFromMaterialID(l_node->getMaterialID())->getColorName()
			);
			l_getColorCases.append(l_caseStatement);
		}
	}
	l_switchStatement.replace(l_switchStatement.find('$'), 1, l_getColorCases);
	l_getColorFunction.replace(l_getColorFunction.find('$'), 1, l_switchStatement);
	return l_getColorFunction;
}

void ShaderGenerator::generateAndSetShader() {

	computeAndSetObjectCount();
	computeUniforms();

	const std::string l_shaderHeader =
	#include "Rendering/Shaders/header.fs"
	"";

	const std::string l_shaderFooter =
	#include "Rendering/Shaders/footer.fs"
	"";

	const std::string l_shaderPrimitive =
	#include "Rendering/Shaders/primitive.fs"
	"";

	std::string l_shaderString;
	int l_index = 0;
	// Appends the shader header which includes a number of utility functions.
	l_shaderString.append(l_shaderHeader);
	// Appends all the uniform declarations.
	l_shaderString.append(ShaderGenerator::generateUniformDeclarations());
	// Appends all the primitive functions.
	l_shaderString.append(l_shaderPrimitive);
	// Appends all the user defined functions.
	appendCustomFunctions(l_shaderString);
	// Generates the getColor function.
	l_shaderString.append(generateGetColorFunction());
	// Generates the isLight function.
	l_shaderString.append(generateIsLightFunction());
	// Generates and appends the object distance functions.
	l_shaderString.append(ShaderGenerator::generateObjectFunctions());
	// Generates and appends the getDistanceFrom function.
	l_shaderString.append(ShaderGenerator::generateGetDistanceFromFunction());
	// Generates and appends the closest object info function.
	l_shaderString.append(ShaderGenerator::generateClosestObjectInfoFunction());
	// Generates and appends the calculate normal function.
	l_shaderString.append(ShaderGenerator::generateCalculateNormalFunction());
	// Generates and appends the get target ray function.
	l_shaderString.append(ShaderGenerator::generateGetTargetRayFunction());
	// Appends the raymarch and main function.
	l_shaderString.append(l_shaderFooter);

	if(m_print_shader)
	std::cout << l_shaderString;

	//std::cout << l_shaderString;
	ShaderGenerator::setShader(l_shaderString);

	m_shaderModified = true;
}

std::string ShaderGenerator::generateUniformDeclarations()
{
	std::string l_uniformDeclarations = "";
	for (std::string l_uniformFloat : m_uniformFloats)
	{
		std::string l_floatTemplate = shader_generation::uniform::UNIFORM_FLOAT;
		l_floatTemplate.replace(l_floatTemplate.find('$'), 1, l_uniformFloat);
		l_uniformDeclarations.append(l_floatTemplate);
	}

	for (std::string vec3 : m_uniformVec3)
	{
		std::string l_vec3Template = shader_generation::uniform::UNIFORM_VEC3;
		l_vec3Template.replace(l_vec3Template.find('$'), 1, vec3);
		l_uniformDeclarations.append(l_vec3Template);
	}
	
	return l_uniformDeclarations;
}

std::string ShaderGenerator::generateGetTargetRayFunction() {
	
	std::string l_targetRayFunction = "\n";
	l_targetRayFunction.append(shader_generation::target_ray::FUNCTION_TEMPLATE);
	l_targetRayFunction.append("\n");

	std::string l_switchStatement = shader_generation::SWITCH_STATEMENT, l_switchContent, l_caseStatement;

	for (int i = 1; i <= ShaderGenerator::m_objectCount; i++) {
		l_caseStatement = shader_generation::target_ray::CASE_STATEMENT;
		l_caseStatement.replace(l_caseStatement.find('$'), 1, std::to_string(i));

		ObjectNode* l_obj = (ObjectNode*)NodeGraph::getSingleton()->getNode(m_objectIDToNodeIDMap[i]);
		Material* l_mat = NodeGraph::getSingleton()->getMaterialFromMaterialID(l_obj->getMaterialID());
		if (l_mat->m_materialType == material_type::METAL)
		{
			std::string l_mettalic_scatter = shader_generation::scatter_calls::METTALIC;
			l_mettalic_scatter = l_mettalic_scatter.replace(l_mettalic_scatter.find('$'), 1, l_mat->get_params_string());
			l_caseStatement.replace(l_caseStatement.find('$'), 1, l_mettalic_scatter);
		}
		else if (l_mat->m_materialType == material_type::DIFFUSE)
		{
			l_caseStatement.replace(l_caseStatement.find('$'), 1, shader_generation::scatter_calls::DIFFUSE);
		}
		else if (l_mat->m_materialType == material_type::LIGHT)
		{
			l_caseStatement.replace(l_caseStatement.find('$'), 1, shader_generation::scatter_calls::LIGHT);
		}
		else if (l_mat->m_materialType == material_type::DIELECTRIC)
		{
			std::string l_dielectric_scatter = shader_generation::scatter_calls::DIELECTRIC;
			l_dielectric_scatter = l_dielectric_scatter.replace(l_dielectric_scatter.find('$'), 1, l_mat->get_params_string());
			l_caseStatement.replace(l_caseStatement.find('$'), 1, l_dielectric_scatter);
		}
		else if (l_mat->m_materialType == material_type::CUSTOM)
		{
			CustomMaterial* l_cmat = (CustomMaterial*)l_mat;
			std::string l_custom_scatter = shader_generation::scatter_calls::CUSTOM;
			l_custom_scatter = l_custom_scatter.replace(l_custom_scatter.find('$'), 1, l_cmat->getCustomName());
			l_custom_scatter = l_custom_scatter.replace(l_custom_scatter.find('$'), 1, l_mat->get_params_string(true));
			l_caseStatement.replace(l_caseStatement.find('$'), 1, l_custom_scatter);
		}
		l_switchContent.append(l_caseStatement);
	}

	l_switchStatement.replace(l_switchStatement.find('$'), 1, l_switchContent);
	l_targetRayFunction.replace(l_targetRayFunction.find('$'), 1, l_switchStatement);

	return l_targetRayFunction;

}

std::string ShaderGenerator::generateGetDistanceFromFunction()
{
	std::string l_generateGetDistanceFromFunction = "\n";
	l_generateGetDistanceFromFunction.append(shader_generation::get_distance_from::FUNCTION_TEMPLATE);
	l_generateGetDistanceFromFunction.append("\n");

	std::string l_switchStatement = shader_generation::SWITCH_STATEMENT, l_switchContent, l_caseStatement;

	for (int i = 1; i <= ShaderGenerator::m_objectCount; i++)
	{
		l_caseStatement = shader_generation::get_distance_from::CASE_STATEMENT;
		for (int j = 0; j < shader_generation::get_distance_from::FREQUENCY; j++) {
			l_caseStatement.replace(l_caseStatement.find('$'), 1, std::to_string(i));
		}

		l_switchContent.append(l_caseStatement);
	}

	l_switchStatement.replace(l_switchStatement.find('$'), 1, l_switchContent);
	l_generateGetDistanceFromFunction.replace(l_generateGetDistanceFromFunction.find('$'), 1, l_switchStatement);

	return l_generateGetDistanceFromFunction;
}

std::string ShaderGenerator::generateIsLightFunction()
{
	std::string l_isLightFunction = "\n";
	l_isLightFunction.append(shader_generation::is_light::FUNCTION_TEMPLATE);
	l_isLightFunction.append("\n");

	std::string l_switchStatement = shader_generation::SWITCH_STATEMENT, l_switchContent, l_caseStatement;

	for (int i = 1; i <= ShaderGenerator::m_objectCount; i++)
	{
		ObjectNode* l_obj = (ObjectNode*)NodeGraph::getSingleton()->getNode(m_objectIDToNodeIDMap[i]);
		Material* l_mat = NodeGraph::getSingleton()->getMaterialFromMaterialID(l_obj->getMaterialID());

		l_caseStatement = shader_generation::is_light::CASE_STATEMENT;
		l_caseStatement.replace(l_caseStatement.find('$'), 1, std::to_string(i));
		if (l_mat->m_materialType == material_type::LIGHT)
		{
			l_caseStatement.replace(l_caseStatement.find('$'), 1, "true");
		}
		else
		{
			l_caseStatement.replace(l_caseStatement.find('$'), 1, "false");
		}
		l_switchContent.append(l_caseStatement);
	}

	l_switchStatement.replace(l_switchStatement.find('$'), 1, l_switchContent);
	l_isLightFunction.replace(l_isLightFunction.find('$'), 1, l_switchStatement);
	return l_isLightFunction;
}

std::string ShaderGenerator::generateCalculateNormalFunction() {
	
	std::string l_calculateNormalFunction = "\n";
	l_calculateNormalFunction.append(shader_generation::calculate_normal::FUNCTION_TEMPLATE);
	l_calculateNormalFunction.append("\n");

	std::string l_switchStatement = shader_generation::SWITCH_STATEMENT, l_switchContent, l_caseStatement;

	for (int i = 1; i <= ShaderGenerator::m_objectCount; i++) {
		l_caseStatement = shader_generation::calculate_normal::CASE_STATEMENT;
		for (int j = 0; j < shader_generation::calculate_normal::FREQUENCY; j++) {
			l_caseStatement.replace(l_caseStatement.find('$'), 1, std::to_string(i));
		}

		l_switchContent.append(l_caseStatement);
	}

	l_switchStatement.replace(l_switchStatement.find('$'), 1, l_switchContent);
	l_calculateNormalFunction.replace(l_calculateNormalFunction.find('$'), 1, l_switchStatement);

	return l_calculateNormalFunction;
}

std::string ShaderGenerator::generateClosestObjectInfoFunction() {
	
	std::string l_closestObjectFunction = "\n";
	l_closestObjectFunction.append(shader_generation::closest_object_info::FUNCTION_TEMPLATE);
	l_closestObjectFunction.append("\n");

	std::string l_functionContent,l_helperString;

	for (int i = 1; i <= ShaderGenerator::m_objectCount; i++) {
		l_helperString = shader_generation::closest_object_info::DISTANCE_COMPUTATION;
		l_helperString.replace(l_helperString.find('$'), 1, std::to_string(i));
		l_helperString.replace(l_helperString.find('$'), 1, std::to_string(i));
		l_functionContent.append(l_helperString);
	}

	l_functionContent.append(shader_generation::closest_object_info::VARIABLE_INITIALIZATION);

	for (int i = 1; i <= ShaderGenerator::m_objectCount; i++) {
		l_helperString = shader_generation::closest_object_info::MIN_DIST_COMPUTATION;
		l_helperString.replace(l_helperString.find('$'), 1, std::to_string(i));
		l_functionContent.append(l_helperString);
	}

	for (int i = 1; i <= ShaderGenerator::m_objectCount; i++) {
		l_helperString = shader_generation::closest_object_info::CONDITIONAL_OBJECT_INDEX_COMPUTATION;
		l_helperString.replace(l_helperString.find('$'), 1, std::to_string(i));
		l_helperString.replace(l_helperString.find('$'), 1, std::to_string(i));
		l_functionContent.append(l_helperString);
	}

	l_closestObjectFunction.replace(l_closestObjectFunction.find('$'), 1, l_functionContent);

	return l_closestObjectFunction;
}


// TODO : refactor code to have small functions
std::string ShaderGenerator::generateObjectFunctions() {
	
	std::string l_objectFunctions;
	NodeGraph* l_nodeGraph = NodeGraph::getSingleton();
	std::vector<int> l_topologicalSorting = l_nodeGraph->getTopologicalSorting();

	for (int l_nodeID : l_topologicalSorting) {
		
		Node* l_nd = l_nodeGraph->getNode(l_nodeID);
		if (l_nd->checkIfTransform() || l_nd->checkIfFinal()) {
			continue;
		}
		std::string l_functionName = "f_" + l_nd->getVariableName();
		std::string l_functionBody = shader_generation::object_function::INITIALIZATION;
		std::string l_nodeFunction = shader_generation::object_function::FUNCTION_TEMPLATE;
		std::string l_returnVariable = l_nd->getVariableName();
		std::string l_returnStatement = shader_generation::RETURN;
		
		int l_index = 0;

		if (l_nd->checkIfObject()) {
			l_functionName = "object";
			l_functionName.append("_");
			l_functionName.append(std::to_string(ShaderGenerator::m_nodeIDToObjectIDMap[l_nd->m_ID]));
		}
		
		l_nodeFunction.replace(l_nodeFunction.find('$'), 1, l_functionName);

		for (auto it : l_nd->m_operationOrdering) {
			l_functionBody.append(shader_generation::object_function::POSITION_RESTORATION);
			std::string l_scalingString = "1.0";
			for (auto itr = it.second.rbegin(); itr < it.second.rend(); itr++) {
				TransformNode* l_transformNode = (*itr);
				if (l_transformNode->m_TransformationType == TransformationType::SCALE) {
					l_scalingString.append(" * ");
					l_scalingString.append(ShaderGenerator::getUniformStringFromLabel(l_transformNode->getVariableName(), l_transformNode->m_inputFloatLabels[0]));
					continue;
				}
				l_functionBody.append(ShaderGenerator::getTransform(*itr));
			}

			std::string l_positionScaling = shader_generation::object_function::POSITION_SCALING;
			l_positionScaling.replace(l_positionScaling.find('$'), 1, l_scalingString);
			l_functionBody.append(l_positionScaling);

			std::string l_valueAssignment = shader_generation::object_function::DISTANCE_STORAGE;
			if (l_nd->m_previousNonTransformNode[l_index] == NULL) {
				l_valueAssignment = "\n";
				l_valueAssignment.append(l_nd->m_getString());
				l_valueAssignment.pop_back();
				l_valueAssignment.append(" * " + l_scalingString);
				l_valueAssignment.append(";\n");
				l_returnVariable = l_nd->getVariableName();
			}
			else {
				std::string l_variableName = l_nd->m_previousNonTransformNode[l_index]->getVariableName();
				std::string l_functionName = "f_" + l_variableName;
				l_variableName.append(std::to_string(l_index));
				l_valueAssignment.replace(l_valueAssignment.find('$'), 1, l_variableName);
				l_valueAssignment.replace(l_valueAssignment.find('$'), 1, l_functionName);
				l_valueAssignment.replace(l_valueAssignment.find('$'), 1, l_scalingString);
				l_returnVariable = l_variableName;
			}

			l_functionBody.append(l_valueAssignment);
			l_index++;
		}

		if (!l_nd->checkIfObject()) {
			l_returnVariable = l_nd->getVariableName();
		}
		l_returnStatement.replace(l_returnStatement.find('$'), 1, l_returnVariable);

		if (l_nd->checkIfOperation()) {
			std::string l_finalStatement = "\n";
			l_finalStatement.append(l_nd->m_getString());
			l_finalStatement.append("\n");
			l_functionBody.append(l_finalStatement);
		}

		l_functionBody.append(l_returnStatement);
		l_nodeFunction.replace(l_nodeFunction.find('#'), 1, l_functionBody);
		l_objectFunctions.append(l_nodeFunction);
	}

	return l_objectFunctions;
}

std::string ShaderGenerator::getUniformStringFromLabel(std::string p_variable_name, std::string p_label){
	std::string l_uniformString = "u_";
	l_uniformString.append(p_variable_name);
	l_uniformString.append("_");
	l_uniformString.append(p_label);
	while (l_uniformString.find(' ') != std::string::npos)
	{
		l_uniformString.replace(l_uniformString.find(' '), 1, "_");
	}
	while (l_uniformString.find('#') != std::string::npos)
	{
		l_uniformString.replace(l_uniformString.find('#'), 1, "_");
	}
	return l_uniformString;
}

std::string ShaderGenerator::getTransform(TransformNode* p_node) {
	
	std::string l_transform, l_postfixForRotation;
	int l_index = 0;

	switch (p_node->m_TransformationType) {
	case TransformationType::TRANSLATION: 
		l_transform = shader_generation::object_function::TRANSLATION_TRANSFORM_APPLICATION;
		break;
	case TransformationType::ROTATION_X:
		l_transform = shader_generation::object_function::ROTATION_TRANSFORM_INIT_X;
		l_postfixForRotation = 'x';
		break;
	case TransformationType::ROTATION_Y:
		l_transform = shader_generation::object_function::ROTATION_TRANSFORM_INIT_Y;
		l_postfixForRotation = 'y';
		break;
	case TransformationType::ROTATION_Z:
		l_transform = shader_generation::object_function::ROTATION_TRANSFORM_INIT_Z;
		l_postfixForRotation = 'z';
		break;
	}

	// Translation requires only the vec3 as parameter, so we replace $ with that uniform's name.
	if (p_node->m_TransformationType == TransformationType::TRANSLATION) {
		l_transform.replace(
			l_transform.find('$'),
			1,
			getUniformStringFromLabel(p_node->getVariableName(), p_node->m_inputFloat3Labels[0])
		);
	}
	else {

		// Rotation requires the theta to be replaced at 2 places, so we replace $ with it in loop.
		while (l_transform.find('$') != std::string::npos) {
			l_transform.replace(
				l_transform.find('$'), 
				1, 
				getUniformStringFromLabel(p_node->getVariableName(), p_node->m_inputFloatLabels[0])
			);
		}
		std::string l_transformApplication = shader_generation::object_function::ROTATION_TRANSFORM_APPLICATION;
		l_transformApplication.replace(l_transformApplication.find('$'), 1, l_postfixForRotation);
		l_transform.append(l_transformApplication);
	}

	return l_transform;
}


