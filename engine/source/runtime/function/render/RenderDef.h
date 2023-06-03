#ifndef __RENDER_DEF_H__
#define __RENDER_DEF_H__

#include <memory>

struct GLFWwindow;

namespace Pionner
{
	class Rhi;
	class WindowSystem;
	class WindowUI;
	class RenderPassBase;
	class RenderPipelineBase;
	class SceneMgr;
	class RenderResourceMgr;
	class ShaderMgr;
	class World;

	struct RenderParam
	{
		std::shared_ptr<SceneMgr>          sceneMgr;
		std::shared_ptr<RenderResourceMgr> resource;
		std::shared_ptr<ShaderMgr>         shaderMgr;
		std::shared_ptr<Rhi>               rhi;
		std::shared_ptr<World>             world;
	};

	struct WindowCreateInfo
	{
		int         width{ 1600 };
		int         height{ 900 };
		const char *title{ "Pionner" };
		bool        fullscreen{ false };
	};

	struct RenderSystemInitInfo
	{
		std::shared_ptr<WindowSystem> window;
	};

	struct RenderPipelineInitInfo
	{
		std::shared_ptr<WindowSystem> window;
	};

	struct RenderPassInitInfo
	{
		std::shared_ptr<Rhi> rhi;
		std::shared_ptr<WindowSystem> window;
	};
}

#endif