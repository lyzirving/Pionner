#ifndef __RENDER_SYSTEM_H__
#define __RENDER_SYSTEM_H__

#include <memory>

namespace Pionner
{
	class Rhi;
	class WindowSystem;
	class WindowUI;

	struct RenderSystemInitInfo
	{
		std::shared_ptr<WindowSystem> m_window;
	};

	class RenderSystem
	{
	public:
		RenderSystem();
		~RenderSystem();

		void initialize(RenderSystemInitInfo &info);
		void initializeUIRenderBackend(WindowUI *windowUI);
		void shutdown();

		void tick(float delta);

	private:
		std::shared_ptr<Rhi> m_rhi;
	};
}

#endif