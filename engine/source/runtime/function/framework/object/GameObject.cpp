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
		auto itr = m_components.begin();
		while (itr != m_components.end())
		{
			(*itr).reset();
			itr = m_components.erase(itr);
		}
	}

	void GameObject::tick(float delta)
	{
	}
}