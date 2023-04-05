#include <cassert>

#define GLEW_STATIC
#include <GL/glew.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "function/render/interface/opengl/RhiOpenGL.h"
#include "function/render/WindowSystem.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RhiOpenGL"

namespace Pionner
{
	RhiOpenGL::RhiOpenGL() : Rhi()
	{
	}

	RhiOpenGL::~RhiOpenGL()
	{
	}

	void RhiOpenGL::initialize(void *param)
	{
		OpenGLRhiInitInfo* info = (OpenGLRhiInitInfo*)param;
		m_window = info->window;

		m_viewport = {0.f, 0.f, float(m_window->getWidth()), float(m_window->getHeight()), 0.f, 1.f};

		createInstance();
	}

	void RhiOpenGL::initUIRenderBackend()
	{
		if (!m_window || !m_window->getWindow())
		{
			LOG_FATAL("rhi's window is invalid");
			assert(0);
		}
		const char* version{ "#version 430" };
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

	void RhiOpenGL::shutdownUIRenderBackend()
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
	}

	void RhiOpenGL::shutdown()
	{
		m_window.reset();
	}

	void RhiOpenGL::createInstance()
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
}