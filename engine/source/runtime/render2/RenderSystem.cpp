#include "RenderSystem.h"

namespace pio
{
	namespace render
	{
		RenderSystem::RenderSystem() : m_scene(new sgf::Scene)
		{
		}

		RenderSystem::~RenderSystem() = default;

		void RenderSystem::tick(uint64_t deltaMs)
		{
			if (m_scene)
			{
				m_scene->tick(deltaMs);
			}
		}

		void RenderSystem::shutdown()
		{
			if (m_scene)
			{
				m_scene->release();
				m_scene.reset();
			}
		}
	}
}