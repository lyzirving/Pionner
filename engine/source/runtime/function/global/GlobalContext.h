#ifndef __GLOBAL_CONTEXT_H__
#define __GLOBAL_CONTEXT_H__

#include <memory>

namespace Pionner
{
	class WindowSystem;
	class RenderSystem;
	class AssetsSystem;
	class EventSystem;

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
		std::shared_ptr<AssetsSystem> m_assetsSystem;
		std::shared_ptr<EventSystem>  m_eventSystem;
	};

	extern GlobalContext g_runtimeCtx;
}

#endif