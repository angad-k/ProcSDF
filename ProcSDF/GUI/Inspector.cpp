#include "GUI/Inspector.h"
#include "GUI/NodeEditor.h"
#include "GUI/NodeGraph.h"
#include "GUI/Nodes/OperationNodes.h"
#include "GUI/Nodes/PrimitiveNodes.h"

void Inspector::initialize()
{
	renderer = Renderer::get_singleton();
}

template<typename p_nodeType>
inline void Inspector::add_node()
{
	Node* new_node = new p_nodeType();
	NodeGraph::get_singleton()->add_node(new_node);
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
	if (ImGui::Button("Node graph settings"))
		openedTab = Tab::NODEGRAPH_SETTINGS;

	

	switch (openedTab)
	{
	case Tab::CAMERA_SETTINGS:
		draw_camera_settings();
		break;
	case Tab::RENDERING_SETTINGS:
		draw_rendering_settings();
		break;
	case Tab::NODEGRAPH_SETTINGS:
		draw_node_graph_settings();
		break;
	}

	ImGui::End();
}

void Inspector::draw_camera_settings()
{
	float cameraOrigin[3] = { renderer->get_camera_origin()[0], renderer->get_camera_origin()[1], renderer->get_camera_origin()[2] };
	ImGui::InputFloat3("Camera Origin", cameraOrigin);
	renderer->set_camera_origin(cameraOrigin);
	ImGui::InputFloat("Focal Length", renderer->get_focal_length());
}

void Inspector::draw_rendering_settings()
{
	ImGui::Text("Something will come here");
}

void Inspector::draw_node_graph_settings()
{
	if (ImGui::Button("Sphere"))
	{
		add_node<SphereNode>();
	}
	if (ImGui::Button("Intersection"))
	{
		add_node<IntersectionNode>();
	}
}