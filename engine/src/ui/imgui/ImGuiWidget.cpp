#include "ImGuiWidget.h"

#include "gfx/renderer/RenderContext.h"

#include <imgui.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ImGuiWidget"

namespace pio
{
	bool WidgetSorter(const Ref<ImGuiWidget>& lhs, const Ref<ImGuiWidget>& rhs)
	{
		if(!lhs)
			return false;

		if(!rhs)
			return true;

		return lhs->DrawOrder() < rhs->DrawOrder();
	}

	ImGuiWidget::ImGuiWidget(const std::string& name) : Object(), m_Name(name)
	{
	}

	void ImGuiWidget::OnAttach(const Ref<Scene>& scene)
	{
		m_Scene = scene;
		for(int32_t i = 0; i < m_Children.size(); ++i)
		{
			m_Children[i]->OnAttach(scene);
		}
	}

	void ImGuiWidget::OnDetach()
	{
		for(int32_t i = m_Children.size() - 1; i >= 0; --i)
		{
			m_Children[i]->OnDetach();
			m_Children.pop_back();
		}
	}	

	void ImGuiWidget::Tick(const Ref<RenderContext>& context)
	{
		if(!IsVisible())
			return;

		OnLayout(context);

		OnTick(context);

		if(!m_Children.empty() && m_NeedSort)
		{
			m_NeedSort = false;
			std::sort(m_Children.begin(), m_Children.end(), WidgetSorter);
		}

		for(size_t i = 0; i < m_Children.size(); i++)
		{
			m_Children[i]->Tick(context);
		}
	}

	void ImGuiWidget::Render(const Ref<RenderContext>& context)
	{
		if(!IsVisible())
			return;

		if(!IsRoot())
		{
			context->SubmitRC([weakContext = WeakRef<RenderContext>(context), 
							   weakSelf = WeakRef<ImGuiWidget>(Self<ImGuiWidget>())]()
			{
				auto ctx = weakContext.lock();
				auto widget = weakSelf.lock();
				if(!(ctx && widget))
					return;

				widget->OnBeforeDraw(ctx);
				widget->OnDraw(ctx);
				widget->OnPostDraw(ctx);
			});
		}

		for(size_t i = 0; i < m_Children.size(); i++)
		{
			m_Children[i]->Render(context);
		}
	}

	void ImGuiWidget::DispatchEvent(const Ref<Event>& event)
	{
		if(!IsVisible())
			return;

		bool consume{ false };
		for(int32_t i = 0; i < m_Children.size(); ++i)
		{
			consume = m_Children[i]->OnEvent(event);
			if(consume)
				break;
		}

		if(!consume)
			consume = OnEvent(event);
	}

	void ImGuiWidget::AddChild(const Ref<ImGuiWidget>& child)
	{
		if(!child || HasChild(child))
			return;

		if(m_Scene.expired())
		{
			LOGE("err! scene has not been attached");
			std::abort();
			return;
		}

		m_NeedSort = true;
		m_Children.push_back(child);
		child->SetParent(Self<ImGuiWidget>());
		child->OnAttach(m_Scene.lock());
	}

	void ImGuiWidget::RemoveChild(const Ref<ImGuiWidget>& child)
	{
		if(!child)
			return;

		auto it = std::find(m_Children.begin(), m_Children.end(), child);
		if(it != m_Children.end())
		{
			(*it)->OnDetach();
			m_Children.erase(it);
			m_NeedSort = true;
		}
	}

	void ImGuiWidget::OnLayout(const Ref<RenderContext>& context)
	{
		if(IsDirty())
		{
			glm::vec2 leftTop{ 0.f };
			if(IsRoot())
			{
				auto* mainVp = ImGui::GetMainViewport();
				m_LayoutParams.Calculate(mainVp->Size.x, mainVp->Size.y);
				leftTop.x = mainVp->Pos.x;
				leftTop.y = mainVp->Pos.y;
			}
			else
			{
				auto p = GetParent();
				m_LayoutParams.Calculate(p->Width(), p->Height());
				leftTop.x = p->Left();
				leftTop.y = p->Top();
			}
			m_Left = leftTop.x + m_LayoutParams.Rect.Left;
			m_Top = leftTop.y + m_LayoutParams.Rect.Top;
			m_Right = m_Left + m_LayoutParams.Rect.Width();
			m_Bottom = m_Top + m_LayoutParams.Rect.Height();

			if(IsRoot())
			{
				Invalidate(false);
			}
		}
	}
}