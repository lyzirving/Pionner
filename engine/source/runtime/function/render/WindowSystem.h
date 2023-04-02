#ifndef __WINDOW_SYSTEM_H__
#define __WINDOW_SYSTEM_H__

struct GLFWwindow;

namespace Pionner
{
	struct WindowCreateInfo
	{
		int         width{ 1280 };
		int         height{ 720 };
		const char* title{ "Pionner" };
		bool        fullscreen{ false };
	};

	class WindowSystem
	{
	public:
		WindowSystem();
		virtual ~WindowSystem();

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }

		void initialize(const WindowCreateInfo& info);
		void shutdown();

	protected:
		static void windowSizeCallback(GLFWwindow *window, int width, int height);
		static void windowCloseCallback(GLFWwindow* window);

	private:
		GLFWwindow* m_window;
		int m_width, m_height;
	};
}

#endif