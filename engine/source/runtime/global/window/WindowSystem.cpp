#include <cassert>
#include <GLFW/glfw3.h>

#include "WindowSystem.h"

#include "global/GlobalContext.h"
#include "global/event/EventMgr.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "WindowSystem"

namespace pio
{
	WindowSystem::WindowSystem() : SystemBase()
	{
	}

	WindowSystem::~WindowSystem() = default;

	void WindowSystem::init(const WindowSystemInitInfo &info)
	{
		if (!glfwInit())
		{
			LOG_FATAL("glfw fail to init");
			assert(0);
		}
		m_width = info.width;
		m_height = info.height;

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(info.width, info.height, info.title, nullptr, nullptr);
		if (!m_window)
		{
			LOG_FATAL("fail to create window");
			glfwTerminate();
			assert(0);
		}
		// set user pointer
		glfwSetWindowUserPointer(m_window, this);

		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);

		// set function callback
		glfwSetWindowSizeCallback(m_window, WindowSystem::windowSizeCallback);
		glfwSetWindowCloseCallback(m_window, WindowSystem::windowCloseCallback);
		glfwSetCursorPosCallback(m_window, WindowSystem::windowMouseMoveCallback);
		glfwSetMouseButtonCallback(m_window, WindowSystem::windowMouseBtnCallback);
		glfwSetScrollCallback(m_window, WindowSystem::windowScrollCallback);

		glfwMakeContextCurrent(m_window);
	}

	void WindowSystem::shutdown()
	{
		if (m_window)
		{
			glfwDestroyWindow(m_window);
			glfwTerminate();
			m_window = nullptr;
		}
	}

	void WindowSystem::windowSizeCallback(GLFWwindow *window, int width, int height)
	{
		WindowSystem *app = (WindowSystem *)glfwGetWindowUserPointer(window);
		app->m_width = width;
		app->m_height = height;
		g_runtimeCtx.m_eventMgr->setWindowSize(width, height);
	}

	void WindowSystem::windowCloseCallback(GLFWwindow *window)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	void WindowSystem::windowMouseMoveCallback(GLFWwindow *window, double xPos, double yPos)
	{
		//LOG_DEBUG("pos[%lf, %lf]", xPos, yPos);
		//WindowSystem *app = (WindowSystem *)glfwGetWindowUserPointer(window);
		g_runtimeCtx.m_eventMgr->setCursorPos(xPos, yPos);
	}

	void WindowSystem::windowMouseBtnCallback(GLFWwindow *window, int button, int action, int mods)
	{
		//LOG_DEBUG("button[%d], action[%d], mods[%d]", button, action, mods);
		//WindowSystem *app = (WindowSystem *)glfwGetWindowUserPointer(window);
		g_runtimeCtx.m_eventMgr->setMotionEvent(button, action, mods);
	}

	void WindowSystem::windowScrollCallback(GLFWwindow *window, double deltaX, double deltaY)
	{
		//LOG_DEBUG("pos[%lf, %lf]", deltaX, deltaY);
		//WindowSystem *app = (WindowSystem *)glfwGetWindowUserPointer(window);
		g_runtimeCtx.m_eventMgr->setScroll(deltaX, deltaY);
	}

	bool WindowSystem::shouldClose()
	{
		if (m_window)
		{
			return glfwWindowShouldClose(m_window);
		}
		else
		{
			return true;
		}
	}

	void WindowSystem::makeCurrent()
	{
		if (m_window)
		{
			glfwMakeContextCurrent(m_window);
		}
	}

	void WindowSystem::swapBuffers()
	{
		if (m_window)
		{
			glfwSwapBuffers(m_window);
		}
		else
		{
			LOG_FATAL("window is null");
			assert(0);
		}
	}

	void WindowSystem::pollEvents()
	{
		glfwPollEvents();
	}
}