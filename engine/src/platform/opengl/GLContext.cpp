#include "GLHeader.h"

#include <GLFW/glfw3.h>

#include "GLContext.h"

#include "core/utils/Profiler.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLContext"

namespace pio
{
	GLContext::GLContext(void *window) : GraphicsContext(), m_winHandle(static_cast<GLFWwindow *>(window))
	{
		PIO_ASSERT(m_winHandle != nullptr, "GLFW window handle is null");
	}

	GLContext::~GLContext()
	{
		m_winHandle = nullptr;
	}

	void GLContext::init()
	{
		PIO_ASSERT_RETURN(m_winHandle != nullptr, "GLFW window handle is null, return");
		glfwMakeContextCurrent(m_winHandle);
	}

	void GLContext::swapBuffer()
	{
		PIO_ASSERT_RETURN(m_winHandle != nullptr, "GLFW window handle is null, return");
		uint64_t start{ PROFILER_TIME };
		glfwSwapBuffers(m_winHandle);
		PROFILERD_DURATION(start, "SwapBuffer");
	}
}