#include "PioWorld.h"

#include "scenegraph/node/Group.h"


namespace pio
{
	uint32_t PioWorld::g_entityId{ 0 };

	PioWorld::PioWorld() : m_dirty(true), m_ecsWorld(), m_entities()
		, m_sceneRoot(new sgf::Group(sgf::ROOT_NODE))
	{
	}

	PioWorld::~PioWorld()
	{
	}

	void PioWorld::init()
	{
	}

	void PioWorld::shutdown()
	{
		auto itr = m_entities.begin();
		while (itr != m_entities.end())
		{
			m_ecsWorld.destroy(itr->second->m_ecsId);
			itr->second.reset();
			itr = m_entities.erase(itr);
		}

		if (m_sceneRoot)
		{
			m_sceneRoot->release();
			m_sceneRoot.reset();
		}
	}
}