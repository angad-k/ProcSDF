#include "GUI/NodeEditor.h"
#include "GUI/Nodes/Node.h"

void NodeEditor::initialize()
{
	// nothing needed for now - we'll add stuff as needed.
	HelloNode* hn = new HelloNode();
	FinalNode* fn = new FinalNode();
	nodes.push_back(hn);
	nodes.push_back(fn);
}

int NodeEditor::allocate_id(Node* p_node)
{
	for (int i = 0; i < INT_MAX; i++)
	{
		if (allocated_ids.find(i) == allocated_ids.end())
		{
			allocated_ids[i] = p_node;
			return i;
		}
	}

	// TO DO : handle too many allocations
}

void NodeEditor::add_link(int src, int dest)
{
	bool possible = true;
	for (auto link : links)
	{
		if (link.second == dest)
		{
			if(! allocated_ids[link.second]->is_final_node)
			{
				possible = false;
			}
		}
	}
	if (possible)
	{
		links.push_back(std::make_pair(src, dest));
	}
	else
	{
		// TO DO : Toasting mechanism needed here.
	}
}

void NodeEditor::add_node(NodeTypes p_nodeType)
{
	Node* new_node = NULL;
	switch (p_nodeType)
	{
	case NodeTypes::HELLO_NODE:
		new_node = new HelloNode();
		break;
	case NodeTypes::SPHERE_PRIMITIVE:
		new_node = new SphereNode();
		break;
	}
	if (new_node)
	{
		nodes.push_back(new_node);
	}
}

void NodeEditor::draw()
{
	ImGui::Begin("Nodes workspace");

	ImNodes::BeginNodeEditor();

	ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;

	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i]->draw();
	}


	for (int i = 0; i < links.size(); ++i)
	{
		const std::pair<int, int> p = links[i];
		ImNodes::Link(i, p.first, p.second);
	}

	ImNodes::EndNodeEditor();

	for (auto i : allocated_ids)
	{
		for (auto j : allocated_ids)
		{
			int start_attr = i.first;
			int end_attr = j.first;
			if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
			{
				add_link(start_attr, end_attr);
			}
		}
	}

	// since we create links with identifier being its index, we can directly use it here.
	for (int i = 0; i < links.size(); i++)
	{
		if (ImNodes::IsLinkDestroyed(&i))
		{
			links.erase(links.begin() + i);
		}
	}


	ImGui::End();
}