#include "function/framework/object/GameObject.h"
#include "function/framework/components/Component.h"
#include "function/framework/event/EventDef.h"

namespace Pionner
{
	std::atomic<uint32_t> GameObject::g_objectId{0};

	GameObject::GameObject(const char* url) 
		: m_id(0)
		, m_name(), m_resUrl(url)
		, m_components()
	{
		m_id = g_objectId.load();
		g_objectId++;
	}

	GameObject::~GameObject()
	{
		for (int i = 0; i < CMP_COUNT; ++i)
		{
			if (m_components[i]) m_components[i].reset();
		}
	}

	bool GameObject::addComponent(const std::shared_ptr<Component>& comp)
	{
		ComponentType type = comp->getType();
		if (!compTypeValid(type)) return false;

		m_components[type] = comp;
		return true;
	}

	std::shared_ptr<Component> GameObject::getComponent(ComponentType type)
	{
		std::shared_ptr<Component> comp{nullptr};
		if (!hasComponent(type))
			return comp;

		comp = m_components[type];
		return comp;
	}

	bool GameObject::hasComponent(ComponentType type)
	{
		if (!compTypeValid(type)) return false;

		return (m_components[type].get() != nullptr);
	}

	void GameObject::tick(float delta)
	{
		// components are ticked under the definition order
		for (int i = 0; i < CMP_COUNT; ++i)
		{
			if (m_components[i])
				m_components[i]->tick(delta);
		}
	}

	void GameObject::tickLogicEvent(const std::shared_ptr<EventSlot>& slot)
	{
		if (hasComponent(slot->m_comp->getType()))
		{
			slot->m_comp->tickLogicEvent(slot->m_type, slot->m_arg);
		}
	}
}