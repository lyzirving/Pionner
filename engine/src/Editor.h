#ifndef __PIONNER_EDITOR_H__
#define __PIONNER_EDITOR_H__

#include "event/EventHub.h"
#include "scene/SceneMgr.h"

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

		void onAttach();
		void onDetach();

		void run();

	public:
		static Editor *Get();

	private:
		static Editor *k_Editor;

	private:
		bool m_running{ true };
		SceneMgr m_sceneMgr;

		Ref<Window> m_window{ nullptr };
		Ref<RenderContext> m_renderCtx{ nullptr };
	};
}

#endif