#include <cassert>
#include <GLFW/glfw3.h>

#include "function/render/WindowSystem.h"
#include "function/framework/world/World.h"
#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "WindowSystem"

namespace Pionner
{
	WindowSystem::WindowSystem(const std::shared_ptr<World> &world)
		: BaseSystem(world)
		, m_window(nullptr)
		, m_width(0), m_height(0)
		, m_sizeChange(false)
	{
	}

	WindowSystem::~WindowSystem() = default;

	void WindowSystem::initialize(const WindowCreateInfo &info)
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
		// set function callback
		glfwSetWindowSizeCallback(m_window, WindowSystem::windowSizeCallback);
		glfwSetWindowCloseCallback(m_window, WindowSystem::windowCloseCallback);

		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);

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
		if (app)
		{
			app->m_width = width;
			app->m_height = height;
			app->setSizeChange(true);
		}
	}

	void WindowSystem::windowCloseCallback(GLFWwindow *window)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
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