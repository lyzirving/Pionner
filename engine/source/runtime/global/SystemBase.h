#ifndef __PIONNER_SYSTEM_BASE_H__
#define __PIONNER_SYSTEM_BASE_H__

#include <memory>

namespace pio
{
	class World;

	class SystemBase
	{
	public:
		SystemBase() {}
		SystemBase(const std::shared_ptr<World> &world) : m_world(world) {}
		virtual ~SystemBase() { m_world.reset(); }

		virtual void tick(float deltaTime) {};

	protected:
		std::shared_ptr<World> m_world{ nullptr };
	};
}

#endif