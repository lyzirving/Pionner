#include "PioEntity.h"

#include "world/PioWorld.h"

#include "scenegraph/node/Node.h"
#include "scenegraph/node/NodeFactory.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PioEntity"

namespace pio
{
	PioEntity::PioEntity(PioEntityType type) 
		: m_id(), m_ecsId()
		, m_world(nullptr)
		, m_key(), m_name(), m_type(type), m_path()
		, m_dirty(true)
		, m_comps()
		, m_sceneNode(nullptr)
	{
	}

	PioEntity::~PioEntity()
	{
		m_sceneNode.reset();
		m_world = nullptr;
	}

	void PioEntity::requestUpdate()
	{
		if (m_world)
		{
			m_world->setDirty(true);
		}
		else
		{
			LOG_ERR("pio world ptr is invalid, entity name[%s], key[%s]",
					m_name.c_str(), m_key.c_str());
		}
	}

	void PioEntity::swapData(float deltaTime)
	{
		if (!m_sceneNode)
		{
			m_sceneNode = scenegrf::NodeFactory::create(m_type);
		}
		m_sceneNode->swapData(*this);
	}
}