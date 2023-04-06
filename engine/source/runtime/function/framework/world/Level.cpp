#include "function/framework/world/Level.h"
#include "function/framework/object/GameObject.h"

namespace Pionner
{
	Level::Level(const char* url) : m_objects(), m_levelResUrl(url)
	{
	}

	Level::~Level()
	{
		shutdown();
	}

	void Level::initialize()
	{
	}

	void Level::shutdown()
	{
		auto itr = m_objects.begin();
		while (itr != m_objects.end())
		{
			itr->second.reset();
			itr = m_objects.erase(itr);
		}
	}

	void Level::tick(float delta)
	{
		auto itr = m_objects.begin();
		while (itr != m_objects.end())
		{
			itr->second->tick(delta);
		}
	}
}