#include "GUI/Inspector.h"
#include "GUI/NodeEditor.h"
#include "GUI/NodeGraph.h"
#include "GUI/Nodes/OperationNodes.h"
#include "GUI/Nodes/PrimitiveNodes.h"
#include "GUI/Nodes/ObjectNode.h"

void Inspector::initialize()
{
	renderer = Renderer::get_singleton();
}

template<typename p_nodeType>
inline int Inspector::add_node()
{
	Node* new_node = new p_nodeType();
	NodeGraph::get_singleton()->add_node(new_node);
	return new_node->id;
}

void Inspector::draw()
{
	ImGui::Begin("Inspector");

	Tab oldTab = openedTab;

	if (oldTab == Tab::CAMERA_SETTINGS)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button("Camera Settings"))
		openedTab = Tab::CAMERA_SETTINGS;
	ImGui::SameLine();
	if (oldTab == Tab::CAMERA_SETTINGS)
	{
		ImGui::EndDisabled();
	}

	if (oldTab == Tab::RENDERING_SETTINGS)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button("Rendering Settings"))
		openedTab = Tab::RENDERING_SETTINGS;
	ImGui::SameLine();
	if (oldTab == Tab::RENDERING_SETTINGS)
	{
		ImGui::EndDisabled();
	}

	if (oldTab == Tab::NODEGRAPH_SETTINGS)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button("Node graph settings"))
		openedTab = Tab::NODEGRAPH_SETTINGS;
	if (oldTab == Tab::NODEGRAPH_SETTINGS)
	{
		ImGui::EndDisabled();
	}

	ImGui::Separator();

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
	if (ImGui::TreeNode("Add nodes"))
	{
		ImGui::Indent();
		
		if (ImGui::TreeNode("Primitives"))
		{
			ImGui::Indent();
			ImGui::PushStyleColor(ImGuiCol_Button, imgui_colors::PRIMITIVE);
			if (ImGui::Button("Sphere"))
			{
				add_node<SphereNode>();
			}
			else if (ImGui::Button("Box"))
			{
				add_node<BoxNode>();
			}
			else if (ImGui::Button("Torus"))
			{
				add_node<TorusNode>();
			}
			else if (ImGui::Button("Box Frame"))
			{
				add_node<BoxFrameNode>();
			}
			ImGui::PopStyleColor();
			ImGui::Unindent();
			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Operations"))
		{
			ImGui::Indent();
			ImGui::PushStyleColor(ImGuiCol_Button, imgui_colors::OPERATION);
			if (ImGui::Button("Intersection"))
			{
				add_node<IntersectionNode>();
			}
			ImGui::PopStyleColor();
			ImGui::Unindent();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Object"))
		{
			ImGui::Indent();
			ImGui::PushStyleColor(ImGuiCol_Button, imgui_colors::OBJECT);
			if (ImGui::Button("Object"))
			{
				int node_id = add_node<ObjectNode>();
				int final_node_id = NodeGraph::get_singleton()->final_node->id;
				//NodeGraph::get_singleton()->add_link(node_id, final_node_id);
				// skipping for now because of some error
			}
			ImGui::PopStyleColor();
			ImGui::Unindent();
			ImGui::TreePop();
		}

		ImGui::Unindent();
		ImGui::TreePop();
	}
	
}