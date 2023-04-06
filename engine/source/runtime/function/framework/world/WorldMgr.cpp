#include <cassert>

#include "function/framework/world/WorldMgr.h"
#include "function/framework/world/Level.h"

namespace Pionner
{
	WorldMgr::WorldMgr() : m_levels(), m_activeLevel(nullptr)
	{
	}

	WorldMgr::~WorldMgr()
	{
		shutdown();
	}

	void WorldMgr::initialize()
	{

	}

	void WorldMgr::shutdown()
	{
		m_activeLevel.reset();
		auto itr = m_levels.begin();
		while (itr != m_levels.end())
		{
			itr->second.reset();
			itr = m_levels.erase(itr);
		}
	}

	void WorldMgr::tick(float delta)
	{
		assert(m_activeLevel.get());

		m_activeLevel->tick(delta);
	}
}