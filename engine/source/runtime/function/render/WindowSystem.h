#ifndef __WINDOW_SYSTEM_H__
#define __WINDOW_SYSTEM_H__

#include "function/render/RenderDef.h"
#include "function/framework/system/BaseSystem.h"

namespace Pionner
{
	class WindowSystem : public BaseSystem
	{
	public:
		WindowSystem(const std::shared_ptr<World> &world);
		virtual ~WindowSystem();

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
		inline GLFWwindow *getWindow() const { return m_window; }

		void initialize(const WindowCreateInfo &info);
		void shutdown();

		bool shouldClose();
		void makeCurrent();
		void swapBuffers();
		void pollEvents();

	protected:
		static void windowSizeCallback(GLFWwindow *window, int width, int height);
		static void windowCloseCallback(GLFWwindow *window);

	private:
		GLFWwindow *m_window;
		int m_width, m_height;
	};
}

#endif