#include "RenderSystem.h"
#include "pass/UiPass.h"

#include "global/event/Event.h"

namespace pio
{
	namespace render
	{
		RenderSystem::RenderSystem(const std::shared_ptr<gfx::GraphicContext> &gfxContext) 
			: m_scene(new sgf::Scene), m_gfxContext(gfxContext)
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

			m_gfxContext.reset();
		}

		void RenderSystem::tick(uint64_t deltaMs)
		{
			sgf::RenderInfo info;
			info.gfxContext = m_gfxContext;
			info.deltaMs = deltaMs;

			if (m_scene)
			{
				m_scene->update(info);
			}

			if (m_uiPass)
			{
				m_uiPass->tick(deltaMs);
			}
		}
		
	}
}