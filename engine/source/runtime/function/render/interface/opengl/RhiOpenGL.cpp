#include <cassert>

#define GLEW_STATIC
#include <GL/glew.h>

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