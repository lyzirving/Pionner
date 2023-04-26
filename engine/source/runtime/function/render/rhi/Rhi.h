#ifndef __RHI_H__
#define __RHI_H__

#include <glm/glm.hpp>

#include "function/render/rhi/RhiDef.h"
#include "function/render/resource/ResourceDef.h"

struct GLFWwindow;

namespace Pionner
{
	class DrawCmd;

	// render hardware interface
	class Rhi
	{
	public:
		Rhi() : m_window(nullptr), m_shaderRhi(nullptr), m_drawCmd(nullptr)
			, m_viewport(), m_type(RHI_COUNT) {}

		virtual ~Rhi()
		{
			m_window.reset();
		}

		virtual void initialize(void *param) = 0;
		virtual void initUIRenderBackend() = 0;
		virtual void shutdownUIRenderBackend() = 0;
		virtual void shutdown() = 0;
		virtual std::shared_ptr<ShaderRhi> getShaderRhi() = 0;
		virtual std::shared_ptr<DrawCmd> getDrawCmd() = 0;

		virtual void viewportFull() {};
		virtual void viewportSub(int x, int y, int width, int height) {};

		inline RhiType getType() const { return m_type; }

	protected:
		virtual void createInstance() {};
		virtual void createWindowSurface() {};

		std::shared_ptr<WindowSystem> m_window;
		std::shared_ptr<ShaderRhi>    m_shaderRhi;
		std::shared_ptr<DrawCmd>      m_drawCmd;
		RhiViewport                   m_viewport;
		RhiType                       m_type;
	};
}

#endif