#include "RuntimeLayer.h"
#include "ImGuiUtils.h"

#include "window/Window.h"
#include "asset/AssetMgr.h"
#include "scene/Scene.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/resource/RenderTarget.h"
#include "gfx/rhi/FrameBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RuntimeLayer"

namespace pio
{
	RuntimeLayer::RuntimeLayer(const LayoutParams& param, const std::string& name) : Layer(param, name)
	{
	}

	void RuntimeLayer::onAttach(Ref<RenderContext>& context)
	{
		auto& window = context->window();
		m_layoutParam.calculate(window->width(), window->height());
	}

	void RuntimeLayer::onDetach()
	{
	}

	void RuntimeLayer::onUpdate(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<RenderTarget>& target)
	{
		onDrawHierarchyView(context, scene, target, LayoutRatio(0.6f, 0.f, 0.75f, 1.f), m_firstTimeShow);
		onDrawInspectorView(context, scene, target, LayoutRatio(0.75f, 0.f, 1.f, 1.f), m_firstTimeShow);
		onDrawSceneView(context, scene, target, LayoutRatio(0.f, 0.f, 0.6f, 1.f), m_firstTimeShow);
		m_firstTimeShow = false;
	}

	bool RuntimeLayer::onEvent(Ref<Event>& event)
	{
		return false;
	}

	void RuntimeLayer::onDrawSceneView(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<RenderTarget>& target, const LayoutRatio& layout, bool firstTime)
	{
		auto& param = m_layoutParam;
		context->submitRC([layout, firstTime, &context, &target, &param]()
		{		
			if (firstTime)
			{
				auto* mainVp = ImGui::GetMainViewport();
				ImVec2 pos = mainVp->Pos;
				pos.x += param.Rect.Left + layout.Left * param.Vp.w();
				pos.y += param.Rect.Top + layout.Top * param.Vp.h();
				ImGui::SetNextWindowPos(pos);
				ImGui::SetNextWindowSize(ImVec2((layout.Right - layout.Left) * param.Vp.w(), (layout.Bottom - layout.Top) * param.Vp.h()));
			}		

			const auto& texture = target->frameBuffer()->colorBuffers()[0];
			ImGui::Begin("Scene", 0, ImGuiUtils::k_FlagCommonWindow);
			auto availSize = ImGui::GetContentRegionAvail();
			float aspect0 = availSize.x / availSize.y;
			float aspect1 = float(texture->width()) / float(texture->height());
			glm::vec2 imgSize;
			//Let image always fills the window size
			if(aspect0 < aspect1)
			{
				imgSize.x = aspect1 * availSize.y;
				imgSize.y = availSize.y;
			}
			else
			{
				imgSize.x = availSize.x;
				imgSize.y = availSize.x / aspect1;
			}
			ImGuiUtils::DrawImage(texture->id(), imgSize, glm::vec2(0.f, 1.f), glm::vec2(1.f, 0.f), 0.f, 0.f);
			ImGui::End();
		});
	}

	void RuntimeLayer::onDrawHierarchyView(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<RenderTarget>& target, const LayoutRatio& layout, bool firstTime)
	{
		auto& param = m_layoutParam;
		auto& selectIdx = m_selectEntIdx;
		context->submitRC([layout, firstTime, &context, &scene, &param, &selectIdx]()
		{
			if (firstTime)
			{
				auto* mainVp = ImGui::GetMainViewport();
				ImVec2 pos = mainVp->Pos;
				pos.x += param.Rect.Left + layout.Left * param.Vp.w();
				pos.y += param.Rect.Top + layout.Top * param.Vp.h();
				ImGui::SetNextWindowPos(pos);
				ImGui::SetNextWindowSize(ImVec2((layout.Right - layout.Left) * param.Vp.w(), (layout.Bottom - layout.Top) * param.Vp.h()));
			}

			const auto& ents = scene->entities();
			ImGui::Begin("Hierachy", 0, ImGuiUtils::k_FlagCommonWindow);
			ImGuiUtils::ShowHierarchy(ents, selectIdx);
			ImGui::End();
		});
	}

	void RuntimeLayer::onDrawInspectorView(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<RenderTarget>& target, const LayoutRatio& layout, bool firstTime)
	{
		auto& param = m_layoutParam;
		uint32_t& selectIdx = m_selectEntIdx;
		context->submitRC([layout, firstTime, &context, &scene, &param, &selectIdx]()
		{
			if (firstTime)
			{
				auto* mainVp = ImGui::GetMainViewport();
				ImVec2 pos = mainVp->Pos;
				pos.x += param.Rect.Left + layout.Left * param.Vp.w();
				pos.y += param.Rect.Top + layout.Top * param.Vp.h();
				ImGui::SetNextWindowPos(pos);
				ImGui::SetNextWindowSize(ImVec2((layout.Right - layout.Left) * param.Vp.w(), (layout.Bottom - layout.Top) * param.Vp.h()));
			}
			ImGui::Begin("Inspector", 0, ImGuiUtils::k_FlagCommonWindow);
			const auto& ent = scene->getEntity(selectIdx);
			if(ent)
			{
				ImGuiUtils::ShowEntity(ent);
			}
			ImGui::End();
		});
	}
}