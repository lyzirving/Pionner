#include "PioEntity.h"
#include "PioWorld.h"

#include "scenegraph/node/Node.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PioEntity"

namespace pio
{
	PioEntity::PioEntity(PioEntityType type, const std::shared_ptr<PioWorld> &world)
		: m_type(type), m_world(world)
	{
	}

	PioEntity::~PioEntity()
	{
		m_sceneNode.reset();
		m_world.reset();
	}

	void PioEntity::requestUpdate()
	{
		m_world->setDirty(true);
	}

	void PioEntity::setName(const std::string &name)
	{
		m_name = name;
		m_sceneNode->setName(name);
	}

	void PioEntity::swapData(float deltaTime)
	{
		m_sceneNode->swapData(*this);
	}
}