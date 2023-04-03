#ifndef __WINDOW_UI_H__
#define __WINDOW_UI_H__

#include <memory>

namespace Pionner
{
	class WindowSystem;
	class RenderSystem;

	struct WindowUIInitInfo
	{
		std::shared_ptr<WindowSystem> windowSystem;
		std::shared_ptr<RenderSystem> renderSystem;
	};

	class WindowUI
	{
	public:
		WindowUI() {};
		virtual ~WindowUI() = default;

		virtual void initialize(WindowUIInitInfo &info) = 0;
		virtual void preRender() = 0;
	};
}

#endif