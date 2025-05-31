#pragma once

#include "ui/UiDef.h"

namespace pio
{
	class Scene;
	class Event;
	class RenderContext;

	class ImGuiWidget : public Object
	{
		RTTR_ENABLE(Object)
	public:
		ImGuiWidget(const std::string& name);
		virtual ~ImGuiWidget() = default;
		virtual void OnAttach(const Ref<Scene>& scene);
		virtual void OnDetach();
		virtual void OnBeforeDraw(const Ref<RenderContext>& context) {}
		virtual void OnDraw(const Ref<RenderContext>& context) {}
		virtual void OnPostDraw(const Ref<RenderContext>& context) { Invalidate(false); }
		virtual bool OnEvent(const Ref<Event>& event) { return false; }

		void Tick(const Ref<RenderContext>& context);
		void Render(const Ref<RenderContext>& context);
		void DispatchEvent(const Ref<Event>& event);

		void AddChild(const Ref<ImGuiWidget>& child);
		void RemoveChild(const Ref<ImGuiWidget>& child);
		bool HasChild(const Ref<ImGuiWidget>& child) const
		{
			return std::find(m_Children.begin(), m_Children.end(), child) != m_Children.end();
		}

		void SetDrawOrder(int32_t order) { m_DrawOrder = order; }
		void SetParent(const Ref<ImGuiWidget>& parent) { m_Parent = parent; }
		void SetVisible(bool visible) { m_Visible = visible; }
		void SetLayoutParam(const LayoutParams& param)
		{
			if(m_LayoutParams != param)
			{
				m_LayoutParams = param;
				Invalidate();
			}
		}
		
		bool IsVisible() const { return m_Visible; }
		bool IsRoot() const { return m_Parent.expired(); }
		bool IsDirty() const { return m_bDirty; }
		bool IsSceneBound() const { return !m_Scene.expired(); }		

		int32_t DrawOrder() const { return m_DrawOrder; }
		Ref<ImGuiWidget> GetParent() const { return m_Parent.lock(); }		
		const LayoutParams& GetLayoutParams() const { return m_LayoutParams; }
		Ref<Scene> GetScene() const { return m_Scene.lock(); }
		void Invalidate(bool val = true) { m_bDirty = val; }

		float Left() const { return m_Left; }
		float Top() const { return m_Top; }
		float Right() const { return m_Right; }
		float Bottom() const { return m_Bottom; }
		float Width() const { return Right() - Left(); }
		float Height() const { return Bottom() - Top(); }
		bool Contain(float x, float y) const { return x >= Left() && x <= Right() && y >= Top() && y <= Bottom(); }
		
	protected:
		virtual void OnLayout(const Ref<RenderContext>& context);
		virtual void OnTick(const Ref<RenderContext>& context) {}

	protected:
		WeakRef<Scene> m_Scene;
		WeakRef<ImGuiWidget> m_Parent;
		std::vector<Ref<ImGuiWidget>> m_Children;

		LayoutParams m_LayoutParams;
		std::string m_Name;
		float m_Left{ 0.f }, m_Top{ 0.f }, m_Right{ 0.f }, m_Bottom{ 0.f };
		bool m_Visible{ true };

		int32_t m_DrawOrder{ -1 };
		std::atomic_bool m_bDirty{ true };
		bool m_NeedSort{ false };		
	};
}