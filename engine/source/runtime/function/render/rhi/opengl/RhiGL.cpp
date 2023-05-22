#include <cassert>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "function/render/rhi/RhiHeader.h"

#include "function/render/rhi/opengl/RhiGL.h"
#include "function/render/rhi/opengl/shader/ShaderRhiGL.h"
#include "function/render/rhi/opengl/cmd/DrawCmdGL.h"

#include "function/render/WindowSystem.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RhiGL"

namespace Pionner
{
	RhiGL::RhiGL() : Rhi()
	{
		m_type = RHI_OPENGL;
	}

	RhiGL::~RhiGL()
	{
	}

	void RhiGL::initialize(void *param)
	{
		OpenGLRhiInitInfo *info = (OpenGLRhiInitInfo *)param;
		m_window = info->window;

		m_viewport = { 0.f, 0.f, float(m_window->getWidth()), float(m_window->getHeight()), 0.f, 1.f };

		createInstance();
	}

	void RhiGL::initUIRenderBackend()
	{
		if (!m_window || !m_window->getWindow())
		{
			LOG_FATAL("rhi's window is invalid");
			assert(0);
		}
		const char *version{ "#version 430" };
		bool ret0 = ImGui_ImplOpenGL3_Init(version);
		bool ret1 = ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);

		if (!ret0 || !ret1)
		{
			LOG_FATAL("fail to init ui render backend, imgui for opengl[%s], imgui glfw[%s]"
					  , ret0 ? "true" : "false", ret1 ? "true" : "false");
			assert(0);
		}

		LOG_DEBUG("Imgui opengl window[%s] inits success", version);
	}

	void RhiGL::shutdownUIRenderBackend()
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
	}

	void RhiGL::shutdown()
	{
		m_window.reset();
	}

	std::shared_ptr<ShaderRhi> RhiGL::getShaderRhi()
	{
		if (!m_shaderRhi.get())
		{
			m_shaderRhi = std::shared_ptr<ShaderRhi>(new ShaderRhiGL);
		}
		return m_shaderRhi;
	}

	std::shared_ptr<DrawCmd> RhiGL::getDrawCmd()
	{
		if (!m_drawCmd.get())
		{
			m_drawCmd = std::shared_ptr<DrawCmd>(new DrawCmdGL);
		}
		return m_drawCmd;
	}

	void RhiGL::createInstance()
	{
		if (glewInit() != GLEW_OK)
		{
			LOG_FATAL("glewInit failed");
			assert(0);
		}
		GLint major{ 0 }, minor{ 0 };
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		LOG_DEBUG("gl version[%d.%d]", major, minor);
	}

	void RhiGL::viewportFull()
	{
		glViewport((GLint)m_viewport.x, (GLint)m_viewport.y, (GLint)m_viewport.width, (GLint)m_viewport.height);
	}

	void RhiGL::viewportSub(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void RhiGL::setClearMode(ClearMode &mode)
	{
		glClearColor(mode.m_color.r, mode.m_color.g, mode.m_color.b, mode.m_color.a);

		uint32_t bit{ 0 };
		if (mode.m_clearColor)
		{
			bit |= GL_COLOR_BUFFER_BIT;
		}
		if (mode.m_clearDepth)
		{
			bit |= GL_DEPTH_BUFFER_BIT;
		}
		if (mode.m_clearStencil)
		{
			bit |= GL_STENCIL_BUFFER_BIT;
		}
		glClear(bit);
	}

	void RhiGL::setCullMode(CullFace &mode)
	{
		if (mode.m_enbale)
		{
			glEnable(GL_CULL_FACE);
			glFrontFace(getGLFaceDir(mode.m_faceDir));
			glCullFace(getGLCullMode(mode.m_mode));
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}

	void RhiGL::setDepthMode(DepthTest &test)
	{
		if (test.m_enbale)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	uint32_t RhiGL::getGLFaceDir(FaceDirection dir)
	{
		switch (dir)
		{
			case CLOCK_WISE:
				return GL_CW;
			case COUNTER_CLOCK_WISE:
				return GL_CCW;
			default:
				return GL_FALSE;
		}
	}

	uint32_t RhiGL::getGLCullMode(CullMode mode)
	{
		switch (mode)
		{
			case CULL_BACK:
				return GL_BACK;
			case CULL_FRONT:
				return GL_FRONT;
			case CULL_FRONT_AND_BACK:
				return GL_FRONT_AND_BACK;
			default:
				return GL_FALSE;
		}
	}
}