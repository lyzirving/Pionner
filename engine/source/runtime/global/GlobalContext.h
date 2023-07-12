#ifndef __GLOBAL_CONTEXT_H__
#define __GLOBAL_CONTEXT_H__

#include <memory>

namespace pio
{
	class EventMgr;
	class WindowSystem;
	class PioWorld;

	namespace render
	{
		class RenderSystem;
	}

	namespace gfx
	{
		class GraphicContext;
	}

	class GlobalContext
	{
	public:
		GlobalContext();
		~GlobalContext();

		void startSystems();
		void swapData(uint64_t deltaMs);
		void shutdownSystems();

	public:
		std::shared_ptr<EventMgr>             m_eventMgr{ nullptr };
		std::shared_ptr<WindowSystem>         m_windowSystem{ nullptr };
		std::shared_ptr<render::RenderSystem> m_render{ nullptr };
		std::shared_ptr<gfx::GraphicContext>  m_gfxContext{ nullptr };
		std::shared_ptr<PioWorld>             m_pioWorld{ nullptr };
	};

	extern GlobalContext g_runtimeCtx;
}

#endif