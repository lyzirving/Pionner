#ifndef __RHI_DEF_H__
#define __RHI_DEF_H__

#include <memory>

namespace Pionner
{
	class WindowSystem;

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