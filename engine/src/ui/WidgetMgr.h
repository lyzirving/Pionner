#pragma once

#include "Common.h"

namespace pio
{
	class Scene;
	class ImGuiWidget;
	class RenderContext;
	class Event;

	class WidgetMgr
	{
	public:
		WidgetMgr(const Ref<RenderContext>& context);
		~WidgetMgr() = default;

		void OnAttach(const Ref<Scene>& scene);
		void OnDetach();

		void DispatchEvent(const Ref<Event>& event);

		void AddWidget(const Ref<ImGuiWidget>& widget);
		void RemoveWidget(const Ref<ImGuiWidget>& widget);

		void Tick(const Ref<RenderContext>& context);

	private:		
		WeakRef<RenderContext> m_Context;
		Ref<ImGuiWidget> m_Root;
	};
}