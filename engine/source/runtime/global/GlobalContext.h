#ifndef __GLOBAL_CONTEXT_H__
#define __GLOBAL_CONTEXT_H__

#include <memory>

namespace pio
{
	class World;
	class WindowSystem;
	class RenderSystem;

	class PioWorld;

	class GlobalContext
	{
	public:
		GlobalContext();
		~GlobalContext();

		void startSystems();
		void swapData(float deltaTime);
		void shutdownSystems();

	public:
		std::shared_ptr<World>        m_world;
		std::shared_ptr<WindowSystem> m_windowSystem;
		std::shared_ptr<RenderSystem> m_renderSystem;

		std::shared_ptr<PioWorld>     m_pioWorld;
	};

	extern GlobalContext g_runtimeCtx;
}

#endif