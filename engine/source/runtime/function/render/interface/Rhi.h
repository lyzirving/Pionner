#ifndef __RHI_H__
#define __RHI_H__

#include "function/render/interface/RhiStruct.h"

struct GLFWwindow;

namespace Pionner
{
	// render hardware interface
	class Rhi
	{
	public:
		Rhi() : m_window(nullptr), m_viewport() {}

		virtual ~Rhi()
		{
			m_window.reset();
		}

		virtual void initialize(void* param) = 0;
		virtual void initUIRenderBackend() = 0;
		virtual void shutdownUIRenderBackend() = 0;
		virtual void shutdown() = 0;

	protected:
		virtual void createInstance() {};
		virtual void createWindowSurface() {};

		std::shared_ptr<WindowSystem> m_window;
		RhiViewport m_viewport;
	};
}

#endif