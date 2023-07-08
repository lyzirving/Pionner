#ifndef __GLOBAL_CONTEXT_H__
#define __GLOBAL_CONTEXT_H__

#include <memory>

namespace pio
{
	class World;
	class WindowSystem;
	class RenderSystem;

	class PioWorld;

	namespace render
	{
		class Render;
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
		std::shared_ptr<World>        m_world;
		std::shared_ptr<WindowSystem> m_windowSystem;
		std::shared_ptr<RenderSystem> m_renderSystem;

		std::shared_ptr<PioWorld>       m_pioWorld{ nullptr };
		std::shared_ptr<render::Render> m_render{ nullptr };
	};

	extern GlobalContext g_runtimeCtx;
}

#endif