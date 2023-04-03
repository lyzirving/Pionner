#ifndef __GLOBAL_CONTEXT_H__
#define __GLOBAL_CONTEXT_H__

#include <memory>

namespace Pionner
{
	class WindowSystem;
	class RenderSystem;

	class GlobalContext
	{
	public:
		GlobalContext();
		~GlobalContext();

		void startSystems();
		void shutdownSystems();

	public:
		std::shared_ptr<WindowSystem> m_windowSystem;
		std::shared_ptr<RenderSystem> m_renderSystem;
	};

	extern GlobalContext g_runtimeCtx;
}

#endif