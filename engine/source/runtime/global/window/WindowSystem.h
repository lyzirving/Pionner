#ifndef __PIONNER_GLOBAL_WINDOW_SYSTEM_H__
#define __PIONNER_GLOBAL_WINDOW_SYSTEM_H__

#include <atomic>
#include "global/SystemBase.h"
#include "render/RenderDef.h"

namespace pio
{
	class EventMgr;

	class WindowSystem : public SystemBase
	{
	public:
		WindowSystem();
		virtual ~WindowSystem();

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
		inline GLFWwindow *getWindow() const { return m_window; }
		// Note: when return type is reference, the returned variable should not be temporaray variable, 
		// it must be member variable.
		inline std::shared_ptr<EventMgr> &getEvtMgr() { return m_evtMgr; }

		void init(const WindowSystemInitInfo &info);
		void shutdown();

		bool shouldClose();
		void makeCurrent();
		void swapBuffers();
		void pollEvents();

		inline bool sizeChange() { return m_sizeChange.load(); }
		inline void setSizeChange(bool change) { m_sizeChange.store(change); }

	protected:
		static void windowSizeCallback(GLFWwindow *window, int width, int height);
		static void windowCloseCallback(GLFWwindow *window);
		static void windowMouseMoveCallback(GLFWwindow *window, double xPos, double yPos);
		static void windowMouseBtnCallback(GLFWwindow *window, int button, int action, int mods);
		static void windowScrollCallback(GLFWwindow *window, double deltaX, double deltaY);

	private:
		GLFWwindow *m_window{ nullptr };
		int m_width{ 0 }, m_height{ 0 };
		std::atomic<bool>         m_sizeChange{ false };
		std::shared_ptr<EventMgr> m_evtMgr{ nullptr };
	};
}

#endif