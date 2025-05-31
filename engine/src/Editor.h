#pragma once

#include "event/EventSocket.h"

namespace pio
{
	class Event;	
	class Window;
	class WindowCloseEvent;
	class SceneMgr;
	class WidgetMgr;
	class RenderContext;
	class RenderPipeline;
	
	class Editor : public EventSocket
	{
	public:
		Editor();
		~Editor();

		void OnEvent(const Ref<Event> &event);
		bool OnWindowClose(const Ref<WindowCloseEvent> &event);

		void OnAttach();
		void OnDetach();

		void Run();

		const Ref<RenderContext>& Context() { return m_Context; }
		const Ref<RenderContext>& Context() const { return m_Context; }		

	protected:
		void Tick();

	public:
		static Editor *Get();

	private:
		static Editor *k_Editor;

	private:
		bool m_Running{ true };

		Ref<Window> m_Window;
		Ref<RenderContext> m_Context;
		Ref<RenderPipeline> m_Pipeline;		
		Ref<SceneMgr> m_SceneMgr;
		Ref<WidgetMgr> m_WidgetMgr;
	};
}