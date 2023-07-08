#include "PioWorld.h"

#include "scenegraph/node/Group.h"


namespace pio
{
	uint32_t PioWorld::g_entityId{ 0 };

	PioWorld::PioWorld() : m_scene(new sgf::Scene)
	{
	}

	PioWorld::~PioWorld()
	{
	}

	void PioWorld::init()
	{
	}

	void PioWorld::swap(uint64_t deltaMs)
	{
		if (dirty())
		{
			auto itr = m_entities.begin();
			while (itr != m_entities.end())
			{
				itr->second->swap(deltaMs);
				itr++;
			}
			setDirty(false);
		}
	}

	void PioWorld::shutdown()
	{
		if (m_scene)
		{
			m_scene->release();
			m_scene.reset();
		}

		auto itr = m_entities.begin();
		while (itr != m_entities.end())
		{
			m_ecsWorld.destroy(itr->second->m_ecsId);
			itr->second.reset();
			itr = m_entities.erase(itr);
		}
	}
}