#ifndef __PIONNER_RENDER_RENDER_SYSTEM_H__
#define __PIONNER_RENDER_RENDER_SYSTEM_H__

#include "scenegraph/view/Scene.h"

namespace pio
{
	class Event;

	namespace gfx
	{
		class GraphicContext;
	}

	namespace render
	{
		class UiPass;

		class RenderSystem
		{
		public:
			RenderSystem(const std::shared_ptr<gfx::GraphicContext> &gfxContext);
			~RenderSystem();

			void dispatchEvent(const Event &event);
			void setWndSize(int32_t width, int32_t height);
			void shutdown();
			void tick(uint64_t deltaMs);

			inline const std::shared_ptr<sgf::Scene> &getScene() { return m_scene; }

		private:
			glm::ivec2                  m_wndSize{ 0, 0 };
			std::shared_ptr<UiPass>     m_uiPass{ nullptr };
			std::shared_ptr<sgf::Scene> m_scene{ nullptr };
			std::shared_ptr<gfx::GraphicContext> m_gfxContext{ nullptr };
		};
	}
}

#endif	