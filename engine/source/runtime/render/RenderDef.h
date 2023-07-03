#ifndef __RENDER_DEF_H__
#define __RENDER_DEF_H__

#include <memory>

struct GLFWwindow;

namespace pio
{
	class Rhi;
	class WindowSystem;
	class WindowUI;
	class RenderPassBase;
	class RenderPipeline;
	class SceneMgr;
	class RenderResourceMgr;
	class ShaderMgr;
	class World;

	struct RenderViewport
	{
		int32_t  m_left;
		int32_t  m_top;
		uint32_t m_width;
		uint32_t m_height;
	};

	struct RenderParam
	{
		std::shared_ptr<SceneMgr>          sceneMgr;
		std::shared_ptr<RenderResourceMgr> resource;
		std::shared_ptr<ShaderMgr>         shaderMgr;
		std::shared_ptr<Rhi>               rhi;
		std::shared_ptr<World>             world;
		RenderViewport                     renderViewport;
		RenderViewport                     windowViewport;
	};

	struct WindowSystemInitInfo
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