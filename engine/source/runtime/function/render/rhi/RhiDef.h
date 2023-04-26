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
}

#endif