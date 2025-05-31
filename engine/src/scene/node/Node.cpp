#include "Node.h"
#include "NodeGroup.h"

#include "scene/Scene.h"
#include "component/TransformComponent.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Node"

namespace pio
{
	uint32_t Node::k_NodeNum = 0;

	Node::Node()
	{
		k_NodeNum++;
	}

	Node::~Node()
	{
		k_NodeNum--;
	}

	int32_t Node::InsertChild(const Ref<Node>& node)
	{
		if(!node)
			return -1;

		for(int32_t i = 0; i < m_Children.size(); ++i)
		{
			if(m_Children[i] == node)
				return i;
		}
		node->SetParent(Self<Node>());
		m_Children.push_back(node);
		return m_Children.size() - 1;
	}

	int32_t Node::RemoveChild(const Ref<Node>& node)
	{
		if(!node)
			return -1;

		for(int32_t i = 0; i < m_Children.size(); i++)
		{
			if(node == m_Children[i])
			{
				m_Children.erase(m_Children.begin() + i);
				return i;
			}
		}
		return -1;
	}

	Ref<Node> Node::FindChild(uint32_t uid) const
	{
		auto it = std::find_if(m_Children.begin(), m_Children.end(), [uid](const Ref<Node>& n)
		{
			return n->UId() == uid;
		});

		if(it != m_Children.end())
		{
			return (*it);
		}
		return Ref<Node>();
	}

	void Node::RemoveAllChildren()
	{
		m_Children.clear();
	}

	void Node::Init()
	{
		AddComponent<TransformComponent>();
		OnInit();
	}

	void Node::AttachActor(const Ref<PhysicsActor>& actor)
	{
		m_Actor = actor;
		OnAttachActor();
	}

	void Node::AttachScene(const Ref<Scene>& scene)
	{
		OnAttachScene(scene);

		for(size_t i = 0; i < m_Children.size(); i++)
			m_Children[i]->AttachScene(scene);
	}

	void Node::DetachScene()
	{
		OnDetachScene();
		
		auto it = m_Components.begin();
		while(it != m_Components.end())
		{
			it->second->OnDetach();
			it = m_Components.erase(it);
		}

		for(size_t i = 0; i < m_Children.size(); i++)
		{
			if(m_Children[i]->IsAttached())
				m_Children[i]->DetachScene();
		}
		m_Children.clear();

		m_Scene.reset();
	}

	void Node::Tick(const Ref<RenderContext>& context)
	{
		if(!IsVisible())
			return;

		OnBeginFrameTick(context);

		OnTickComponents(context);

		OnTick(context);

		OnEndFrameTick(context);

		OnTickChildren(context);
	}

	void Node::Render(const Ref<RenderContext>& context)
	{
		if(!IsVisible())
			return;

		OnRender(context);

		OnRenderChildren(context);
	}

	void Node::SetPosition(const glm::vec3& position)
	{
		GetComponent<TransformComponent>()->SetPosition(position);
	}

	void Node::SetRotation(const glm::vec3& rotation)
	{
		GetComponent<TransformComponent>()->SetRotation(rotation);
	}

	void Node::SetScale(const glm::vec3& scale)
	{
		GetComponent<TransformComponent>()->SetScale(scale);
	}

	void Node::SetMobility(Mobility mobility)
	{
		GetComponent<TransformComponent>()->SetMobility(mobility);
	}

	const glm::vec3& Node::GetPosition() const
	{
		return GetComponent<TransformComponent>()->GetPosition();
	}

	const Rotator& Node::GetRotator() const
	{
		return GetComponent<TransformComponent>()->GetRotation();
	}

	const glm::vec3& Node::GetScale() const
	{
		return GetComponent<TransformComponent>()->GetScale();
	}

	Mobility Node::GetMobility() const
	{
		return GetComponent<TransformComponent>()->GetMobility();
	}

	void Node::SetGroupIndex(int32_t index)
	{
		if(m_GroupIndex != index)
		{
			m_GroupIndex = index;
			OnGroupIndexChange();
		}
	}

	bool Node::IsInGroup() const
	{
		return IsChild() && m_Parent.lock()->Is<NodeGroup>();
	}

	void Node::OnTickComponents(const Ref<RenderContext>& context)
	{
		for(auto comp : m_Components)
		{
			comp.second->OnTick();
		}
	}

	void Node::OnTickChildren(const Ref<RenderContext>& context)
	{
		for(auto& child : m_Children)
		{
			child->Tick(context);
		}
	}

	void Node::OnRenderChildren(const Ref<RenderContext>& context)
	{
		for(auto& child : m_Children)
		{
			child->Render(context);
		}
	}
}