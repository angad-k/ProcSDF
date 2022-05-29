#include "NodeEditor.h"

void NodeEditor::initialize()
{
	// nothing needed for now - we'll add stuff as needed.
}

void NodeEditor::draw()
{
	ImGui::Begin("Nodes workspace");

	ImNodes::BeginNodeEditor();

	ImNodes::BeginNode(1);

	const int output_attr_id = 2;
	ImNodes::BeginOutputAttribute(output_attr_id);

	ImGui::Button("Hellu");
	ImGui::Text("output pin");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();

	ImNodes::EndNodeEditor();

	ImGui::End();
}