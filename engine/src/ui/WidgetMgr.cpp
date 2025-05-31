#include "WidgetMgr.h"
#include "ui/imgui/ImGuiWidget.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "WidgetMgr"

namespace pio
{
	WidgetMgr::WidgetMgr(const Ref<RenderContext>& context) : m_Context(context)
	{		
	}

	void WidgetMgr::OnAttach(const Ref<Scene>& scene)
	{
		m_Root = CreateRef<ImGuiWidget>("Root");
		m_Root->OnAttach(scene);
	}

	void WidgetMgr::OnDetach()
	{
		m_Root->OnDetach();
		m_Root.reset();
	}

	void WidgetMgr::DispatchEvent(const Ref<Event>& event)
	{
		m_Root->DispatchEvent(event);
	}

	void WidgetMgr::AddWidget(const Ref<ImGuiWidget>& widget)
	{
		m_Root->AddChild(widget);
	}

	void WidgetMgr::RemoveWidget(const Ref<ImGuiWidget>& widget)
	{
		m_Root->RemoveChild(widget);
	}

	void WidgetMgr::Tick(const Ref<RenderContext>& context)
	{
		m_Root->Tick(context);
		m_Root->Render(context);
	}
}