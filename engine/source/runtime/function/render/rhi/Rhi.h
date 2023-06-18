#ifndef __RHI_H__
#define __RHI_H__

#include <glm/glm.hpp>
#include <list>

#include "function/render/resource/ResourceDef.h"

#include "function/render/rhi/RhiDef.h"
#include "function/render/rhi/DrawCmd.h"

#include "function/render/rhi/attr/CullFace.h"
#include "function/render/rhi/attr/DepthTest.h"
#include "function/render/rhi/attr/Blend.h"

#include "function/render/WindowSystem.h"

struct GLFWwindow;

namespace Pionner
{
	// render hardware interface
	class Rhi
	{
	public:
		Rhi() : m_window(nullptr), m_shaderRhi(nullptr), m_drawCmd(nullptr), m_type(RHI_COUNT)
		{}

		virtual ~Rhi()
		{
			m_window.reset();
		}

		virtual void initialize(void *param) = 0;
		virtual void initUIRenderBackend() = 0;
		virtual void shutdownUIRenderBackend() = 0;
		virtual void shutdown() = 0;
		virtual std::shared_ptr<ShaderRhi> getShaderRhi() = 0;
		virtual std::shared_ptr<DrawCmd>   getDrawCmd() = 0;

		virtual void clear(uint32_t flags) {};
		virtual void clearColor(const glm::vec4 &color) {};
		virtual void setViewport(int x, int y, int width, int height) {};

		virtual void setCullMode(CullFace &mode) {};
		virtual void setDepthMode(DepthTest &test) {};
		virtual void setBlendMode(Blend &blend) {};

		inline RhiType getType() const { return m_type; }
		inline std::shared_ptr<WindowSystem> getWindowSystem() const { return m_window; }

	protected:
		virtual void createInstance() {};
		virtual void createWindowSurface() {};

		std::shared_ptr<WindowSystem> m_window;
		std::shared_ptr<ShaderRhi>    m_shaderRhi;
		std::shared_ptr<DrawCmd>      m_drawCmd;

		RhiType                       m_type;
	};
}

#endif