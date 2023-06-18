#ifndef __RHI_DEF_H__
#define __RHI_DEF_H__

#include <memory>

namespace Pionner
{
	class WindowSystem;
	class ShaderRhi;

	static const uint32_t COLOR_BUF_BIT = 0x00004000;
	static const uint32_t DEPTH_BUF_BIT = 0x00000100;
	static const uint32_t STENCIL_BUF_BIT = 0x00000400;

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
}

#endif