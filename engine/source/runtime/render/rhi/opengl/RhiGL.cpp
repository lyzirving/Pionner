#include <cassert>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "render/rhi/RhiHeader.h"

#include "render/rhi/opengl/RhiGL.h"
#include "render/rhi/opengl/cmd/DrawCmdGL.h"

#include "global/window/WindowSystem.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RhiGL"

namespace pio
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

	void RhiGL::clear(uint32_t flags)
	{
		glClear(flags);
	}

	void RhiGL::clearColor(const glm::vec4 &color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RhiGL::setViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void RhiGL::setCullMode(gfx::CullFace &mode)
	{
		if (mode.m_enable)
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

	void RhiGL::setDepthMode(gfx::DepthTest &test)
	{
		if (test.m_enable)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void RhiGL::setBlendMode(gfx::Blend &blend)
	{
		if (blend.m_enable)
		{
			glEnable(GL_BLEND);
			glBlendFunc(getGLBlendFactor(blend.m_src),
						getGLBlendFactor(blend.m_dest));
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void RhiGL::unbindBufSlot(BufferType type)
	{
		switch (type)
		{
			case pio::BUF_TEXTURE:
				glBindTexture(GL_TEXTURE_2D, 0);
				break;
			case pio::BUF_CUBE_TEXTURE:
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				break;
			case pio::BUF_DEPTH_FBO:
			case pio::BUF_CUBE_DEPTH_FBO:
			case pio::BUF_COLOR_FRAMEBUFFER:
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				break;
			default:
				break;
		}
	}

	uint32_t RhiGL::getGLFaceDir(gfx::FaceDirection dir)
	{
		switch (dir)
		{
			case gfx::CLOCK_WISE:
				return GL_CW;
			case gfx::COUNTER_CLOCK_WISE:
				return GL_CCW;
			default:
				return GL_FALSE;
		}
	}

	uint32_t RhiGL::getGLCullMode(gfx::CullMode mode)
	{
		switch (mode)
		{
			case gfx::CULL_BACK:
				return GL_BACK;
			case gfx::CULL_FRONT:
				return GL_FRONT;
			case gfx::CULL_FRONT_AND_BACK:
				return GL_FRONT_AND_BACK;
			default:
				return GL_FALSE;
		}
	}

	uint32_t RhiGL::getGLBlendFactor(gfx::BlendFactor blendFactor)
	{
		switch (blendFactor)
		{
			case pio::gfx::Bld_Factor_Zero:
				return GL_ZERO;
			case pio::gfx::Bld_Factor_One:
				return GL_ONE;
			case pio::gfx::Bld_Factor_SrcColor:
				return GL_SRC_COLOR;
			case pio::gfx::Bld_Factor_OneMinusSrcColor:
				return GL_ONE_MINUS_SRC_COLOR;
			case pio::gfx::Bld_Factor_SrcAlpha:
				return GL_SRC_ALPHA;
			case pio::gfx::Bld_Factor_OneMinusSrcAlpha:
				return GL_ONE_MINUS_SRC_ALPHA;
			case pio::gfx::Bld_Factor_DstAlpha:
				return GL_DST_ALPHA;
			case pio::gfx::Bld_Factor_OneMinusDstAlpha:
				return GL_ONE_MINUS_DST_ALPHA;
			case pio::gfx::Bld_Factor_DstColor:
				return GL_DST_COLOR;
			case pio::gfx::Bld_Factor_OneMinusDstColor:
				return GL_ONE_MINUS_DST_COLOR;
			case pio::gfx::Bld_Factor_SrcAlphaSaturate:
				return GL_SRC_ALPHA_SATURATE;
			case pio::gfx::Bld_Factor_ConstantColor:
				return GL_CONSTANT_COLOR;
			case pio::gfx::Bld_Factor_OneMinusConstantColor:
				return GL_ONE_MINUS_CONSTANT_COLOR;
			case pio::gfx::Bld_Factor_ConstantAlpha:
				return GL_CONSTANT_ALPHA;
			case pio::gfx::Bld_Factor_OneMinusConstantAlpha:
				return GL_ONE_MINUS_CONSTANT_ALPHA;
			default:
				return 0;
		}
	}
}