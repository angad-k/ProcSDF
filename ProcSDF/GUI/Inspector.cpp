#include "GUI/Inspector.h"
#include "GUI/NodeEditor.h"
#include "GUI/NodeGraph.h"
#include "GUI/Nodes/OperationNodes.h"
#include "GUI/Nodes/PrimitiveNodes.h"
#include "GUI/Nodes/ObjectNode.h"
#include "GUI/Nodes/TransformNodes.h"
#include "GuiUtilities.h"

void Inspector::initialize()
{
	m_renderer = Renderer::getSingleton();
}

template<typename p_nodeType>
inline Node* Inspector::addNode()
{
	Node* l_newNode = new p_nodeType();
	NodeGraph::getSingleton()->addNode(l_newNode);
	return l_newNode;
}

void Inspector::draw()
{
	ImGui::Begin("Inspector");

	Tab l_oldTab = m_openedTab;

	if (l_oldTab == Tab::CAMERA_SETTINGS)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button("Camera Settings"))
		m_openedTab = Tab::CAMERA_SETTINGS;
	ImGui::SameLine();
	if (l_oldTab == Tab::CAMERA_SETTINGS)
	{
		ImGui::EndDisabled();
	}

	if (l_oldTab == Tab::RENDERING_SETTINGS)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button("Rendering Settings"))
		m_openedTab = Tab::RENDERING_SETTINGS;
	ImGui::SameLine();
	if (l_oldTab == Tab::RENDERING_SETTINGS)
	{
		ImGui::EndDisabled();
	}

	if (l_oldTab == Tab::NODEGRAPH_SETTINGS)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button("Node graph settings"))
		m_openedTab = Tab::NODEGRAPH_SETTINGS;
	if (l_oldTab == Tab::NODEGRAPH_SETTINGS)
	{
		ImGui::EndDisabled();
	}

	ImGui::Separator();

	switch (m_openedTab)
	{
	case Tab::CAMERA_SETTINGS:
		drawCameraSettings();
		break;
	case Tab::RENDERING_SETTINGS:
		drawRenderingSettings();
		break;
	case Tab::NODEGRAPH_SETTINGS:
		drawNodeGraphSettings();
		break;
	}

	ImGui::End();
}

void Inspector::drawCameraSettings()
{
	float p_cameraOrigin[3] = { m_renderer->get_camera_origin()[0], m_renderer->get_camera_origin()[1], m_renderer->get_camera_origin()[2] };
	ImGui::DragFloat3("Camera Origin", p_cameraOrigin);
	m_renderer->setCameraOrigin(p_cameraOrigin);
	ImGui::DragFloat("Focal Length", m_renderer->getFocalLength());
}

void Inspector::drawRenderingSettings()
{
	ImGui::Text("Something will come here");
}

void Inspector::drawNodeGraphSettings()
{
	if (ImGui::TreeNode("Add nodes"))
	{
		ImGui::Indent();
		
		const int l_approximateButtonSize = 85;

		if (ImGui::TreeNode("Primitives"))
		{
			ImGui::Indent();
			ImGui::PushStyleColor(ImGuiCol_Button, imgui_colors::PRIMITIVE);
			if (ImGui::Button("Sphere"))
			{
				addNode<SphereNode>();
			}
			GUI_Utilities::appendToSameLineIfApplicable(l_approximateButtonSize);
			if (ImGui::Button("Box"))
			{
				addNode<BoxNode>();
			}
			GUI_Utilities::appendToSameLineIfApplicable(l_approximateButtonSize);
			if (ImGui::Button("Torus"))
			{
				addNode<TorusNode>();
			}
			GUI_Utilities::appendToSameLineIfApplicable(l_approximateButtonSize);
			if (ImGui::Button("Box Frame"))
			{
				addNode<BoxFrameNode>();
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
				addNode<IntersectionNode>();
			}
			GUI_Utilities::appendToSameLineIfApplicable(l_approximateButtonSize);
			if (ImGui::Button("Union"))
			{
				addNode<UnionNode>();
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
				Node* object_node = addNode<ObjectNode>();
				int final_node_id = NodeGraph::getSingleton()->m_finalNode->m_ID;
				NodeGraph::getSingleton()->addLink(object_node->m_outputIDs[0], 
					NodeGraph::getSingleton()->m_finalNode->m_inputIDs[0]);
			}
			ImGui::PopStyleColor();
			ImGui::Unindent();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Transform"))
		{
			ImGui::Indent();
			ImGui::PushStyleColor(ImGuiCol_Button, imgui_colors::TRANFSFORM);
			if (ImGui::Button("Translation"))
			{
				addNode<TranslationNode>();
			}
			GUI_Utilities::appendToSameLineIfApplicable(l_approximateButtonSize);
			if (ImGui::Button("Rotation Around X-Axis"))
			{
				addNode<RotationNodeX>();
			}
			GUI_Utilities::appendToSameLineIfApplicable(l_approximateButtonSize);
			if (ImGui::Button("Rotation Around Y-Axis"))
			{
				addNode<RotationNodeY>();
			}
			GUI_Utilities::appendToSameLineIfApplicable(l_approximateButtonSize);
			if (ImGui::Button("Rotation Around Z-Axis"))
			{
				addNode<RotationNodeZ>();
			}
			ImGui::PopStyleColor();
			ImGui::Unindent();
			ImGui::TreePop();
		}

		ImGui::Unindent();
		ImGui::TreePop();
	}
	
}