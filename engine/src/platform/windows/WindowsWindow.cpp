#include <GLFW/glfw3.h>

#include "WindowsWindow.h"

#include "event/AppEvent.h"
#include "event/MouseEvent.h"
#include "event/KeyCodes.h"
#include "event/EventMgr.h"

#include "gfx/renderer/RenderContext.h"

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
		switch(mode)
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

	WindowsWindow::WindowsWindow(const WindowProps &prop) : Window(), m_Props(prop)
	{
	}

	bool WindowsWindow::Init()
	{
		m_Data.m_title = m_Props.Title;
		m_Data.m_width = m_Props.Width;
		m_Data.m_height = m_Props.Height;

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			PIO_CHECK_RETURN_FALSE(success == GLFW_TRUE, "err! fail to init glfw[%d]", success);
			glfwSetErrorCallback(WindowErrorCallback);
		}

		if (m_Props.Backend == RenderBackend_OpenGL)
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only					
		}

		m_Window = glfwCreateWindow(m_Data.m_width, m_Data.m_height, m_Data.m_title.c_str(), nullptr, nullptr);
		s_GLFWWindowCount++;
		LOGD("create glfw window[%u]", s_GLFWWindowCount);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		glfwSetWindowSizeCallback(m_Window, WindowSizeCallback);
		glfwSetWindowCloseCallback(m_Window, WindowCloseCallback);		
		glfwSetScrollCallback(m_Window, WindowScrollCallback);

		glfwSetKeyCallback(m_Window, WindowKeyCallback);
		glfwSetMouseButtonCallback(m_Window, WindowMouseBtnCallback);
		glfwSetCursorPosCallback(m_Window, WindowCursorPosCallback);
		return true;
	}

	void WindowsWindow::Shutdown()
	{
		if (m_Window)
		{
			LOGD("destroy window[%u]", s_GLFWWindowCount);
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
			s_GLFWWindowCount--;
		}

		if (s_GLFWWindowCount == 0)
		{
			LOGD("glfw terminate");
			glfwTerminate();
		}
	}

	void WindowsWindow::MakeCurrent()
	{
		PIO_CHECK_RETURN(m_Window != nullptr, "err! GLFW window handle is null");
		glfwMakeContextCurrent(m_Window);
	}

	void WindowsWindow::SwapBuffer()
	{
		PIO_CHECK_RETURN(m_Window != nullptr, "err! GLFW window handle is null");
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::PollEvents()
	{	
		glfwPollEvents();
	}

	glm::ivec2 WindowsWindow::Position() const
	{
		glm::ivec2 pos{ 0 };
		if (m_Window)
		{
			glfwGetWindowPos(m_Window, &pos.x, &pos.y);
		}
		return pos;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		//TODO: why vsync is implementd by this?
		if(enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.m_vsync = enabled;
	}

	void WindowsWindow::SetCursorMode(CursorMode mode)
	{
		if(m_Window && m_CursorMode != mode)
		{
			m_CursorMode = mode;
			glfwSetInputMode(m_Window, GLFW_CURSOR, ToGLFWCursorMode(mode));
			if(mode == CursorMode::Disabled)
			{
				// force the window gain the focus when cursor is disabled
				glfwFocusWindow(m_Window);
			}
		}
	}

	void WindowsWindow::WindowErrorCallback(int error, const char *description)
	{
		LOGE("GLFW error[%d][%s]!", error, description);
	}

	void WindowsWindow::WindowSizeCallback(GLFWwindow *window, int width, int height)
	{
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		data.m_width = width;
		data.m_height = height;

		Ref<Event> event = CreateRef<WindowResizeEvent>(width, height);
		EventMgr::Get()->PushBackWin(event);		
	}

	void WindowsWindow::WindowCloseCallback(GLFWwindow *window)
	{
		Ref<Event> event = CreateRef<WindowCloseEvent>();
		EventMgr::Get()->PushBackWin(event);
	}

	void WindowsWindow::WindowScrollCallback(GLFWwindow* window, double deltaX, double deltaY)
	{
		Ref<Event> event = CreateRef<MouseScrolledEvent>((float)deltaX, (float)deltaY);
		EventMgr::Get()->PushBackWin(event);
	}

	void WindowsWindow::WindowKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		switch(action)
		{
			case GLFW_PRESS:
			case GLFW_REPEAT:
			{
				Key::Set(key);
				break;
			}
			case GLFW_RELEASE:
			{
				Key::Release();
				break;
			}
			default:
				LOGD("unimplemented key action[%d], key[%d]", action, key);
				break;
		}
	}

	void WindowsWindow::WindowMouseBtnCallback(GLFWwindow *window, int button, int action, int mods)
	{
		switch(action)
		{
			case GLFW_PRESS:
			{
				Mouse::Set(button);
				break;
			}
			case GLFW_RELEASE:
			{
				Mouse::Release();
				break;
			}
			default:
				LOGD("unimplemented mouse action[%d], button[%d]", action, button);
				break;
		}
	}

	void WindowsWindow::WindowCursorPosCallback(GLFWwindow* window, double posX, double posY)
	{
		Mouse::SetPosition(posX, posY);
	}
}