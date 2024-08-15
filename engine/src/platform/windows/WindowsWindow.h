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
		~WindowsWindow() = default;

	public:
		virtual bool init() override;
		virtual void shutdown() override;

		virtual void makeCurrent() override;
		virtual void swapBuffer() override;

		virtual void pollEvents() override;
		virtual void setEventCallback(const EvtCbFn &callback) override { m_data.cbFunc = callback; }
		virtual glm::ivec2 position() const override;		
		virtual glm::vec2 cursor() const override { return m_data.m_cursorPos; }
		virtual uint32_t width()  const override  { return m_data.m_width; }
		virtual uint32_t height() const override { return m_data.m_height; }
		virtual void* nativeWindow() const override { return m_window; }	

		virtual void setVSync(bool enabled) override;
		virtual void setCursorMode(CursorMode mode) override;

		virtual bool isVSync() const override { return m_data.m_vsync; }

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
		struct WindowData
		{
			std::string  m_title{};
			uint32_t     m_width, m_height;
			bool         m_vsync{false};
			glm::vec2    m_cursorPos{ 0.f };
			EvtCbFn      cbFunc;
		};

	private:
		WindowProps m_props{};
		GLFWwindow *m_window{ nullptr };
		WindowData m_data{};
		CursorMode m_cursorMode{ CursorMode::Normal };
	};
}

#endif