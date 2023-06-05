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

	struct RhiViewport
	{
		float x, y;
		float width, height;
		float minDepth, maxDepth;
	};

	struct ViewportState
	{
		float x, y;
		float width, height;

		ViewportState() : x(0), y(0), width(0), height(0) {}

		ViewportState &operator=(const RhiViewport &rhiViewport)
		{
			this->x = rhiViewport.x;
			this->y = rhiViewport.y;
			this->width = rhiViewport.width;
			this->height = rhiViewport.height;
			return *this;
		}
	};
}

#endif