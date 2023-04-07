#include "function/framework/object/GameObject.h"

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
			if (m_components[i])
				m_components[i].reset();
		}
	}

	void GameObject::tick(float delta)
	{
	}
}