#ifndef __PIONNER_BASE_SYSTEM_H__
#define __PIONNER_BASE_SYSTEM_H__

#include <memory>

namespace Pionner
{
	class World;

	class BaseSystem
	{
	public:
		BaseSystem(const std::shared_ptr<World> &world) : m_world(world) {}
		virtual ~BaseSystem() { m_world.reset(); }

		virtual void tick(float deltaTime) {};

	protected:
		std::shared_ptr<World> m_world;
	};
}

#endif