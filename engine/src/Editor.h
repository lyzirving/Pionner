#ifndef __PIONNER_EDITOR_H__
#define __PIONNER_EDITOR_H__

#include "core/Base.h"

namespace pio
{
	class Event;
	class Window;
	class RenderContext;

	class Editor
	{
	public:
		Editor();
		~Editor();

		void onEvent(Event &event);
		bool onWindowClose(Event &event);
		bool onWindowResize(Event &event);

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