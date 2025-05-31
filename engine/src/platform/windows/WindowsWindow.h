#pragma once

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
		virtual bool Init() override;
		virtual void Shutdown() override;

		virtual void MakeCurrent() override;
		virtual void SwapBuffer() override;

		virtual void PollEvents() override;
		virtual glm::ivec2 Position() const override;		
		virtual glm::vec2 Cursor() const override { return m_Data.m_cursorPos; }
		virtual uint32_t Width()  const override  { return m_Data.m_width; }
		virtual uint32_t Height() const override { return m_Data.m_height; }
		virtual void* NativeWindow() const override { return m_Window; }	

		virtual void SetVSync(bool enabled) override;
		virtual void SetCursorMode(CursorMode mode) override;

		virtual bool IsVSync() const override { return m_Data.m_vsync; }

	public:
		static void WindowErrorCallback(int error, const char *description);
		static void WindowSizeCallback(GLFWwindow *window, int width, int height);
		static void WindowCloseCallback(GLFWwindow *window);
		static void WindowScrollCallback(GLFWwindow* window, double deltaX, double deltaY);

		static void WindowKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
		static void WindowMouseBtnCallback(GLFWwindow *window, int button, int action, int mods);
		static void WindowCursorPosCallback(GLFWwindow *window, double posX, double posY);

	private:
		struct WindowData
		{
			std::string  m_title{};
			uint32_t     m_width, m_height;
			bool         m_vsync{ false };
			glm::vec2    m_cursorPos{ 0.f };
		};

	private:
		WindowProps m_Props{};
		GLFWwindow *m_Window{ nullptr };
		WindowData m_Data{};
		CursorMode m_CursorMode{ CursorMode::Normal };
	};
}