#include "Inspector.h"
#include "NodeEditor.h"

void Inspector::initialize()
{
	renderer = Renderer::get_singleton();
}

void Inspector::draw()
{
	ImGui::Begin("Inspector");

	if (ImGui::Button("Camera Settings"))
		openedTab = Tab::CAMERA_SETTINGS;
	ImGui::SameLine();
	if (ImGui::Button("Rendering Settings"))
		openedTab = Tab::RENDERING_SETTINGS;
	ImGui::SameLine();
	if (ImGui::Button("Add Nodes"))
		openedTab = Tab::ADD_NODES;

	float cameraOrigin[3] = { renderer->get_camera_origin()[0], renderer->get_camera_origin()[1], renderer->get_camera_origin()[2] };

	switch (openedTab)
	{
	case Tab::CAMERA_SETTINGS:
		ImGui::InputFloat3("Camera Origin", cameraOrigin);
		renderer->set_camera_origin(cameraOrigin);

		//float focalLength = renderer->get_focal_length();
		ImGui::InputFloat("Focal Length", renderer->get_focal_length());
		break;
	case Tab::RENDERING_SETTINGS:
		ImGui::Text("Something will come here");
		break;
	case Tab::ADD_NODES:
		if (ImGui::Button("Hello Node"))
		{
			NodeEditor::get_singleton()->add_node(NodeTypes::HELLO_NODE);
		}
		if (ImGui::Button("Sphere"))
		{
			NodeEditor::get_singleton()->add_node(NodeTypes::SPHERE_PRIMITIVE);
		}
		break;
	}

	ImGui::End();
}