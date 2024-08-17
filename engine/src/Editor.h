#ifndef __PIONNER_EDITOR_H__
#define __PIONNER_EDITOR_H__

#include "base/Base.h"
#include "event/EventHub.h"

namespace pio
{
	class Event;
	class Window;
	class RenderContext;
	class WindowCloseEvent;
	class WindowResizeEvent;

	class Editor : public EventHubObj
	{
	public:
		Editor();
		~Editor();

		void onEvent(Ref<Event> &event);
		bool onWindowClose(Ref<WindowCloseEvent> &event);
		bool onWindowResize(Ref<WindowResizeEvent> &event);

		void onInit();
		void onQuit();

		void run();

	public:
		static Editor *Get();

	private:
		static Editor *k_Editor;

	private:
		bool m_running{ true };
		Ref<Window> m_window{ nullptr };
		Ref<RenderContext> m_renderCtx{ nullptr };
	};
}

#endif