#ifndef __PIONNER_EDITOR_H__
#define __PIONNER_EDITOR_H__

#include "event/EventHub.h"

namespace pio
{
	class Event;	
	class Window;
	class WindowCloseEvent;
	class WindowResizeEvent;

	class RenderContext;
	class RenderPipeline;

	class LayerMgr;
	class SceneMgr;

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

	protected:
		void onPrepare();

	public:
		static Editor *Get();

	private:
		static Editor *k_Editor;

	private:
		bool m_running{ true };

		Ref<LayerMgr> m_layerMgr;
		Ref<SceneMgr> m_sceneMgr;

		Ref<Window> m_window{ nullptr };
		Ref<RenderContext>  m_context{ nullptr };
		Ref<RenderPipeline> m_pipeline{ nullptr };
	};
}

#endif