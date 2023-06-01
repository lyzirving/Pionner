#ifndef __WINDOW_UI_H__
#define __WINDOW_UI_H__

#include <memory>

namespace Pionner
{
	class WindowSystem;
	class RenderSystem;
	class SceneMgr;

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
		virtual void draw(std::shared_ptr<SceneMgr> &sceneMgr) = 0;
	};
}

#endif