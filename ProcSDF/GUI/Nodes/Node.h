#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <map>
#include "Common/constant.h"
class TransformNode;
class Node {
public:
	int m_ID;
	virtual void draw();
	void drawStartNode();
	void drawEndNode();
	void drawNodeContents();
	std::string m_getString();

	std::vector <std::string> m_inputPins;
	std::vector <int> m_inputIDs;
	std::vector <std::string> m_outputPins;
	std::vector <int> m_outputIDs;
	std::map<int, std::vector<TransformNode*>> m_operationOrdering;

	std::vector <std::string> m_inputFloat3Labels;
	std::vector <std::vector<float>> m_inputFloat3;
	std::vector <std::string> m_inputFloatLabels;
	std::vector <float> m_inputFloats;

	std::string m_nodeName;
	std::string m_variableName;
	int m_titleColor = imgui_colors::BLUE;
	int m_visitCount;

	std::vector<Node*> m_previousNonTransformNode;

	std::string getVariableName()
	{
		return m_variableName;
	}
	void init();
	void init(int p_ID);

	bool checkIfTransform();
	bool checkIfOperation();
	bool checkIfPrimitive();
	bool checkIfObject();
	bool checkIfFinal();

	~Node();
};
