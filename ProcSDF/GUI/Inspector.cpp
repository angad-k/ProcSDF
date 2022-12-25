#include "GUI/Inspector.h"
#include "GUI/NodeEditor.h"
#include "GUI/NodeGraph.h"
#include "GUI/Nodes/OperationNodes.h"
#include "GUI/Nodes/PrimitiveNodes.h"
#include "GUI/Nodes/ObjectNode.h"
#include "GUI/Nodes/TransformNodes.h"
#include "GUI/Nodes/CustomNode.h"
#include "GuiUtilities.h"
#include "Common/os.h"
#include "Rendering/Materials/CustomMaterial.h"

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

	if (l_oldTab == Tab::WORLD_SETTINGS)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button("World Settings"))
		m_openedTab = Tab::WORLD_SETTINGS;
	if (l_oldTab == Tab::WORLD_SETTINGS)
	{
		ImGui::EndDisabled();
	}
	ImGui::SameLine();

	if (l_oldTab == Tab::RENDERING_SETTINGS)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button("Rendering Settings"))
		m_openedTab = Tab::RENDERING_SETTINGS;
	if (l_oldTab == Tab::RENDERING_SETTINGS)
	{
		ImGui::EndDisabled();
	}
	ImGui::SameLine();

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
	ImGui::SameLine();

	if (l_oldTab == Tab::MATERIAL_SETTINGS)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button("Material settings"))
		m_openedTab = Tab::MATERIAL_SETTINGS;
	if (l_oldTab == Tab::MATERIAL_SETTINGS)
	{
		ImGui::EndDisabled();
	}

	ImGui::Separator();

	switch (m_openedTab)
	{
	case Tab::WORLD_SETTINGS:
		drawWorldSettings();
		break;
	case Tab::RENDERING_SETTINGS:
		drawRenderingSettings();
		break;
	case Tab::NODEGRAPH_SETTINGS:
		drawNodeGraphSettings();
		break;
	case Tab::MATERIAL_SETTINGS:
		drawMaterialSettings();
		break;
	}
	
	ImGui::End();
}

void Inspector::drawWorldSettings()
{
	float p_cameraOrigin[3] = { m_renderer->getCameraOrigin()[0], m_renderer->getCameraOrigin()[1], m_renderer->getCameraOrigin()[2] };
	ImGui::DragFloat3("Camera Origin", p_cameraOrigin);
	m_renderer->setCameraOrigin(p_cameraOrigin);
	ImGui::DragFloat("Focal Length", m_renderer->getFocalLength());
	ImGui::Separator();
	ImGui::ColorEdit3("Horizon Top Color", m_renderer->m_horizon_top_color);
	ImGui::ColorEdit3("Horizon Bottom Color", m_renderer->m_horizon_bottom_color);
}

void Inspector::drawRenderingSettings()
{
	if (ImGui::TreeNode("Rendering parameters"))
	{
		ImGui::Indent();
		std::vector<std::string> l_render_uniforms = Renderer::getSingleton()->getRenderUniforms();
		for (int i = 0; i < l_render_uniforms.size(); i++)
		{
			ImGui::DragInt(l_render_uniforms[i].c_str(), &Renderer::getSingleton()->m_render_uniforms_values[i]);
		}
		ImGui::Unindent();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Debug Settings"))
	{
		ImGui::Indent();
		std::vector<std::string> l_render_uniforms_debug = Renderer::getSingleton()->getRenderUniformsDebug();
		for (int i = 0; i < l_render_uniforms_debug.size(); i++)
		{
			bool value = Renderer::getSingleton()->m_render_uniforms_debug_values[i];
			if (i > 0 && !Renderer::getSingleton()->m_render_uniforms_debug_values[0])
			{
				ImGui::BeginDisabled();
			}
			ImGui::Checkbox(l_render_uniforms_debug[i].c_str(), &value);
			if (i > 0 && !Renderer::getSingleton()->m_render_uniforms_debug_values[0])
			{
				ImGui::EndDisabled();
			}
			Renderer::getSingleton()->m_render_uniforms_debug_values[i] = value;
		}
		ImGui::Unindent();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Image Export"))
	{
		ImGui::Indent();

		ImGui::DragInt2("Render Size", m_renderSize);

		if (ImGui::Button("Export"))
		{
			std::pair<bool, std::string> l_renderTo = OS::pickRenderToFile();
			if (l_renderTo.first)
			{
				Renderer::getSingleton()->exportImage(l_renderTo.second, m_renderSize[0], m_renderSize[1]);
			}
		}

		ImGui::Unindent();
		ImGui::TreePop();
	}
	
}

void Inspector::drawMaterialSettings()
{
	std::vector<Material*> l_materials = NodeGraph::getSingleton()->getMaterials();
	for (int i = 0; i < l_materials.size(); i++)
	{
		bool l_del = false;
		ImGui::PushID(i);
		l_materials[i]->draw(l_del);
		ImGui::PopID();

		if (l_del)
		{
			NodeGraph::getSingleton()->deleteMaterialAt(i);
			NodeGraph::getSingleton()->fixMaterials();
		}
	}
	if (ImGui::Button("Add Diffuse Material"))
	{
		NodeGraph::getSingleton()->addDiffuse();
	}
	if (ImGui::Button("Add Metal Material"))
	{
		NodeGraph::getSingleton()->addMetal();
	}
	if (ImGui::Button("Add Dielectric Material"))
	{
		NodeGraph::getSingleton()->addDielectric();
	}
	if (ImGui::Button("Add Light(Emissive) Material"))
	{
		NodeGraph::getSingleton()->addLight();
	}
	ImGui::Separator();
	if (ImGui::Button("Add custom material"))
	{
		std::pair<bool, std::string> l_pickFileResult = OS::pickFile();
		if (l_pickFileResult.first)
		{
			std::string l_filePath = l_pickFileResult.second;
			CustomMaterial::AddCustomMaterialAtFilePath(l_filePath);
		}
	}
	if (ImGui::TreeNode("Add Custom Materials"))
	{
		ImGui::Indent();
		std::vector <std::string> l_customMaterialNames = NodeGraph::getSingleton()->getCustomMaterialNames();
		for (int i = 0; i < l_customMaterialNames.size(); i++)
		{
			if (ImGui::Button(l_customMaterialNames[i].c_str()))
			{
				CustomMaterial* l_customMaterial = new CustomMaterial(l_customMaterialNames[i]);
				if (l_customMaterial->isMalformed())
				{
					delete(l_customMaterial);
				}
				else
				{
					NodeGraph::getSingleton()->addMaterial(l_customMaterial);
				}
			}
		}
		ImGui::Unindent();
		ImGui::TreePop();
	}
}

void Inspector::drawNodeGraphSettings()
{
	if (ImGui::Button("Add custom node"))
	{
		std::pair<bool, std::string> l_pickFileResult = OS::pickFile();
		if (l_pickFileResult.first)
		{
			std::string l_filePath = l_pickFileResult.second;
			CustomNode::AddCustomNodeAtFilePath(l_filePath);
		}
	}
	ImGui::Separator();
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
		if (ImGui::TreeNode("Custom Nodes"))
		{
			ImGui::Indent();
			ImGui::PushStyleColor(ImGuiCol_Button, imgui_colors::CUSTOM_NODE);
			std::vector <std::string> l_customNodeNames = NodeGraph::getSingleton()->getCustomNodeNames();
			for (int i = 0; i < l_customNodeNames.size(); i++)
			{
				if (ImGui::Button(l_customNodeNames[i].c_str()))
				{
					CustomNode* l_customNode = new CustomNode(l_customNodeNames[i]);
					if (l_customNode->isMalformed())
					{
						delete(l_customNode);
					}
					else
					{
						NodeGraph::getSingleton()->addNode(l_customNode);
					}
				}
				if (i < l_customNodeNames.size() - 1)
				{
					GUI_Utilities::appendToSameLineIfApplicable(l_approximateButtonSize);
				}
			}
			ImGui::PopStyleColor();
			ImGui::Unindent();
			ImGui::TreePop();
		}

		ImGui::Unindent();
		ImGui::TreePop();
	}
	
}