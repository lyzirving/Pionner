#ifndef __GLOBAL_CONTEXT_H__
#define __GLOBAL_CONTEXT_H__

#include <memory>

namespace Pionner
{
	class WindowSystem;
	class Rhi;

	class GlobalContext
	{
	public:
		GlobalContext();
		~GlobalContext();

		void startSystems();
		void shutdownSystems();

	public:
		std::shared_ptr<WindowSystem> m_windowSystem;
		std::shared_ptr<Rhi>          m_rhi;
	};

	extern GlobalContext g_runtimeCtx;
}

#endif