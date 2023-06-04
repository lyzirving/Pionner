#ifndef __WINDOW_SYSTEM_H__
#define __WINDOW_SYSTEM_H__

#include <atomic>
#include "function/render/RenderDef.h"
#include "function/framework/system/BaseSystem.h"

namespace Pionner
{
	class EventMgr;

	class WindowSystem : public BaseSystem
	{
	public:
		WindowSystem(const std::shared_ptr<World> &world);
		virtual ~WindowSystem();

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
		inline GLFWwindow *getWindow() const { return m_window; }
		inline std::shared_ptr<EventMgr> getEvtMgr() { return m_evtMgr; }

		void initialize(const WindowSystemInitInfo &info);
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
		static void windowScrollCallback(GLFWwindow *window, double xPos, double yPos);

	private:
		GLFWwindow *m_window;
		int m_width, m_height;
		std::atomic<bool>         m_sizeChange;
		std::shared_ptr<EventMgr> m_evtMgr;
	};
}

#endif