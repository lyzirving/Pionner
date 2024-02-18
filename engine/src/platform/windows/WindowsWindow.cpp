#include <GLFW/glfw3.h>

#include "WindowsWindow.h"

#include "window/event/AppEvent.h"
#include "window/event/KeyEvent.h"
#include "window/event/MouseEvent.h"

#include "gfx/rhi/RenderAPI.h"
#include "gfx/renderer/Renderer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "WindowsWindow"

namespace pio
{
	// we might open multiple glfw window
	static uint8_t s_GLFWWindowCount = 0;

	static uint32_t ToGLFWCursorMode(CursorMode mode)
	{
		switch (mode)
		{
			case CursorMode::Hidden:
				return GLFW_CURSOR_HIDDEN;
			case CursorMode::Disabled:
				return GLFW_CURSOR_DISABLED;
			case CursorMode::Normal:
			default:
				return GLFW_CURSOR_NORMAL;
		}
	}

	WindowsWindow::WindowsWindow(const WindowProps &prop)
	{
		init(prop);
	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::pollEvents()
	{
		glfwPollEvents();
	}

	void WindowsWindow::getWindowPos(int32_t &x, int32_t &y)
	{
		if (m_window)
		{
			glfwGetWindowPos(m_window, &x, &y);
		}
		else
		{
			LOGE("native window is invalid");
			x = y = 0;
		}
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		//TODO: why vsync is implementd by this?
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_data.m_vsync = enabled;
	}

	void WindowsWindow::setCursorMode(CursorMode mode)
	{
		if (m_window && m_cursorMode != mode)
		{
			m_cursorMode = mode;
			glfwSetInputMode(m_window, GLFW_CURSOR, ToGLFWCursorMode(mode));
			if (mode == CursorMode::Disabled)
			{
				// force the window gain the focus when cursor is disabled
				glfwFocusWindow(m_window);
			}
		}
	}

	void WindowsWindow::init(const WindowProps &prop)
	{
		m_data.m_title = prop.m_title;
		m_data.m_width = prop.m_width;
		m_data.m_height = prop.m_height;

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			PIO_ASSERT_RETURN(success == GLFW_TRUE, "fail to init glfw[%d], return!", success);
			glfwSetErrorCallback(windowErrorCallback);
		}

		if (RenderAPI::IsOpenGL())
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only					
		}

		m_window = glfwCreateWindow(m_data.m_width, m_data.m_height, m_data.m_title.c_str(), nullptr, nullptr);
		s_GLFWWindowCount++;
		LOGD("create glfw window[%u]", s_GLFWWindowCount);

		glfwSetWindowUserPointer(m_window, &m_data);
		glfwSetWindowSizeCallback(m_window, windowSizeCallback);
		glfwSetWindowCloseCallback(m_window, windowCloseCallback);
		glfwSetKeyCallback(m_window, windowKeyCallback);
		glfwSetCharCallback(m_window, windowCharCallback);
		glfwSetMouseButtonCallback(m_window, windowMouseBtnCallback);
		glfwSetScrollCallback(m_window, windowScrollCallback);
		glfwSetCursorPosCallback(m_window, windowCursorPosCallback);
	}

	void WindowsWindow::shutdown()
	{
		if (m_window)
		{
			LOGD("destroy window[%u]", s_GLFWWindowCount);
			glfwDestroyWindow(m_window);
			m_window = nullptr;
			s_GLFWWindowCount--;
		}

		if (s_GLFWWindowCount == 0)
		{
			LOGD("glfw terminate");
			glfwTerminate();
		}
	}

	void WindowsWindow::windowErrorCallback(int error, const char *description)
	{
		LOGE("GLFW error[%d][%s]!", error, description);
	}

	void WindowsWindow::windowSizeCallback(GLFWwindow *window, int width, int height)
	{
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		data.m_width = width;
		data.m_height = height;

		WindowResizeEvent event(width, height);
		data.cbFunc(event);
	}

	void WindowsWindow::windowCloseCallback(GLFWwindow *window)
	{
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		data.cbFunc(event);
	}

	void WindowsWindow::windowKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key);
				data.cbFunc(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.cbFunc(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true);
				data.cbFunc(event);
				break;
			}
			default:
				break;
		}
	}

	void WindowsWindow::windowCharCallback(GLFWwindow *window, unsigned int keycode)
	{
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		KeyTypedEvent event(keycode);
		data.cbFunc(event);
	}

	void WindowsWindow::windowMouseBtnCallback(GLFWwindow *window, int button, int action, int mods)
	{
		if (Renderer::UI_HasFocus(UiFocus::MouseCapture))
			return;

		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		switch (action)
		{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.cbFunc(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.cbFunc(event);
				break;
			}
			default:
				break;
		}
	}

	void WindowsWindow::windowScrollCallback(GLFWwindow *window, double deltaX, double deltaY)
	{
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		MouseScrolledEvent event((float)deltaX, (float)deltaY);
		data.cbFunc(event);
	}

	void WindowsWindow::windowCursorPosCallback(GLFWwindow *window, double xPos, double yPos)
	{
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		MouseMovedEvent event((float)xPos, (float)yPos);
		data.m_cursorPos.x = xPos;
		data.m_cursorPos.y = yPos;
		data.cbFunc(event);
	}
}