#include "RenderSystem.h"
#include "pass/UiPass.h"

#include "global/event/Event.h"

namespace pio
{
	namespace render
	{
		RenderSystem::RenderSystem() : m_scene(new sgf::Scene)
		{
		}

		RenderSystem::~RenderSystem() = default;

		void RenderSystem::dispatchEvent(const Event &event)
		{
		}

		void RenderSystem::setWndSize(int32_t width, int32_t height)
		{
			if (m_wndSize.x == width && m_wndSize.y == height)
				return;

			m_wndSize.x = width; m_wndSize.y = height;

			if (m_scene)
			{
				m_scene->setWndSize(width, height);
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

		void RenderSystem::tick(uint64_t deltaMs)
		{
			if (m_scene)
			{
				m_scene->tick(deltaMs);
			}
			if (m_uiPass)
			{
				m_uiPass->tick(deltaMs);
			}
		}
		
	}
}