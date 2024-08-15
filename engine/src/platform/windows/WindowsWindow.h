#ifndef __PIONNER_PLATFORM_WINDOWS_WINDOWS_WINDOW_H__
#define __PIONNER_PLATFORM_WINDOWS_WINDOWS_WINDOW_H__

#include "window/Window.h"

struct GLFWwindow;

namespace pio
{
	/*
	* Windows window is specialized for glfw
	*/
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps &prop);
		~WindowsWindow();

	public:
		virtual void pollEvents() override;
		virtual void setEventCallback(const EvtCbFn &callback) override { m_data.cbFunc = callback; }
		virtual void getWindowPos(int32_t &x, int32_t &y) override;

		virtual uint32_t getWidth()  const override  { return m_data.m_width; }
		virtual uint32_t getHeight() const override { return m_data.m_height; }
		virtual void *getNativeWindow() const override { return m_window; }
		virtual glm::vec2 getCursorPos() const override { return m_data.m_cursorPos; }
		virtual bool isVSync() const override { return m_data.m_vsync; }

		virtual void setVSync(bool enabled) override;
		virtual void setCursorMode(CursorMode mode) override;

	public:
		static void windowErrorCallback(int error, const char *description);
		static void windowSizeCallback(GLFWwindow *window, int width, int height);
		static void windowCloseCallback(GLFWwindow *window);
		static void windowKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
		static void windowCharCallback(GLFWwindow *window, unsigned int keycode);
		static void windowMouseBtnCallback(GLFWwindow *window, int button, int action, int mods);
		static void windowScrollCallback(GLFWwindow *window, double deltaX, double deltaY);
		static void windowCursorPosCallback(GLFWwindow *window, double xPos, double yPos);

	private:
		void init(const WindowProps &prop);
		void shutdown();

	private:
		struct WindowData
		{
			std::string  m_title{};
			uint32_t     m_width, m_height;
			bool         m_vsync{false};
			glm::vec2    m_cursorPos{ 0.f };
			EvtCbFn      cbFunc;
		};

	private:
		GLFWwindow *m_window{ nullptr };
		WindowData m_data{};
		CursorMode m_cursorMode{ CursorMode::Normal };
	};
}

#endif