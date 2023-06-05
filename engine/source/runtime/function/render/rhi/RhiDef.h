#ifndef __RHI_DEF_H__
#define __RHI_DEF_H__

#include <memory>

namespace Pionner
{
	class WindowSystem;
	class ShaderRhi;

	enum RhiType : uint8_t
	{
		RHI_OPENGL,
		RHI_VULKAN,
		RHI_COUNT
	};

	struct OpenGLRhiInitInfo
	{
		std::shared_ptr<WindowSystem> window;
	};

	struct Viewport
	{
		float x, y;
		float width, height;

		Viewport() : x(0), y(0), width(0), height(0) {}
		Viewport(float posX, float posY, float portWidth, float portHeight)
			: x(posX), y(posY), width(portWidth), height(portHeight) {}
	};
}

#endif