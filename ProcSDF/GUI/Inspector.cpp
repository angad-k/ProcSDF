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
inline Node* Inspector::add_node()
{
	Node* new_node = new p_nodeType();
	NodeGraph::get_singleton()->add_node(new_node);
	return new_node;
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
	ImGui::DragFloat3("Camera Origin", cameraOrigin);
	renderer->set_camera_origin(cameraOrigin);
	ImGui::DragFloat("Focal Length", renderer->get_focal_length());
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
		
		const int approximate_button_size = 85;

		if (ImGui::TreeNode("Primitives"))
		{
			ImGui::Indent();
			ImGui::PushStyleColor(ImGuiCol_Button, imgui_colors::PRIMITIVE);
			if (ImGui::Button("Sphere"))
			{
				add_node<SphereNode>();
			}
			append_to_same_line_if_applicable(approximate_button_size);
			if (ImGui::Button("Box"))
			{
				add_node<BoxNode>();
			}
			append_to_same_line_if_applicable(approximate_button_size);
			if (ImGui::Button("Torus"))
			{
				add_node<TorusNode>();
			}
			append_to_same_line_if_applicable(approximate_button_size);
			if (ImGui::Button("Box Frame"))
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
				//int node_id = add_node<ObjectNode>();
				Node* object_node = add_node<ObjectNode>();
				int final_node_id = NodeGraph::get_singleton()->final_node->id;
				NodeGraph::get_singleton()->add_link(object_node->output_ids[0], 
					NodeGraph::get_singleton()->final_node->input_ids[0]);
			}
			ImGui::PopStyleColor();
			ImGui::Unindent();
			ImGui::TreePop();
		}

		ImGui::Unindent();
		ImGui::TreePop();
	}
	
}