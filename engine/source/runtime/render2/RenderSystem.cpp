#include "RenderSystem.h"
#include "pass/UiPass.h"

#include "global/event/Event.h"
#include "global/window/ui/WindowUI.h"

namespace pio
{
	namespace render
	{
		RenderSystem::RenderSystem(const std::shared_ptr<gfx::GraphicContext> &gfxContext) 
			: m_scene(new sgf::Scene), m_gfxContext(gfxContext)
		{
		}

		RenderSystem::~RenderSystem() = default;

		void RenderSystem::attachUi(const std::shared_ptr<WindowUI> &ui)
		{
			if (!m_uiPass)
				m_uiPass = std::make_shared<UiPass>();

			m_uiPass->attachUi(ui);
		}

		void RenderSystem::dispatchEvent(const Event &event)
		{
		}

		void RenderSystem::setWndSize(int32_t width, int32_t height)
		{
			if (m_wndSize.x == width && m_wndSize.y == height)
				return;

			m_wndSize.x = width; m_wndSize.y = height;

			m_uiPass->setWndSize(width, height);
			ViewLayout main = m_uiPass->getMainLayout();
			glm::vec2 size{ main.m_width, main.m_height };
			glm::vec2 pos{ main.m_left, m_wndSize.y - main.m_top - main.m_height };
			
			m_scene->setViewport(pos.x, pos.y, size.x, size.y);
		}

		void RenderSystem::shutdown()
		{
			if (m_scene)
			{
				m_scene->release();
				m_scene.reset();
			}

			if (m_uiPass)
			{
				m_uiPass->release();
				m_uiPass.reset();
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
				m_uiPass->update(info);
			}
		}
		
	}
}