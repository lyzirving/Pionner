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
	class Camera;
	class Frustum;

	struct RenderParam
	{
		std::shared_ptr<Camera>  camera;
		std::shared_ptr<Frustum> frustum;
	};

	struct WindowCreateInfo
	{
		int         width{ 1280 };
		int         height{ 720 };
		const char *title{ "Pionner" };
		bool        fullscreen{ false };
	};

	struct RenderSystemInitInfo
	{
		std::shared_ptr<WindowSystem> window;
	};

	struct RenderPipelineInitInfo
	{
	};

	struct RenderPassInitInfo
	{
	};

	struct RenderPassCommonInfo
	{
		std::shared_ptr<Rhi> rhi;
	};
}

#endif