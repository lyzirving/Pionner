#ifndef __PIONNER_GLOBAL_WINDOW_SYSTEM_H__
#define __PIONNER_GLOBAL_WINDOW_SYSTEM_H__

#include "global/SystemBase.h"
#include "global/GlobalDef.h"

struct GLFWwindow;

namespace pio
{
	class WindowSystem : public SystemBase
	{
	public:
		WindowSystem();
		virtual ~WindowSystem();

		inline int  getWidth() const { return m_width; }
		inline int  getHeight() const { return m_height; }
		inline GLFWwindow *getWindow() const { return m_window; }

		void init(const WindowSystemInitInfo &info);
		void shutdown();

		bool shouldClose();
		void makeCurrent();
		void swapBuffers();
		void pollEvents();

	protected:
		static void windowSizeCallback(GLFWwindow *window, int width, int height);
		static void windowCloseCallback(GLFWwindow *window);
		static void windowMouseMoveCallback(GLFWwindow *window, double xPos, double yPos);
		static void windowMouseBtnCallback(GLFWwindow *window, int button, int action, int mods);
		static void windowScrollCallback(GLFWwindow *window, double deltaX, double deltaY);

	private:
		GLFWwindow *m_window{ nullptr };
		int        m_width{ 0 }, m_height{ 0 };
	};
}

#endif