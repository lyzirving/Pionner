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
			ImGuiUtils::DrawImage(texture->id(), glm::vec2(availSize.x, availSize.y), glm::vec2(0.f, 1.f), glm::vec2(1.f, 0.f), 0.f, 0.f);
			ImGui::End();
		});
	}

	void RuntimeLayer::onDrawHierarchyView(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<RenderTarget>& target, const LayoutRatio& layout, bool firstTime)
	{
		auto& param = m_layoutParam;
		context->submitRC([layout, firstTime, &context, &param]()
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

			ImGui::Begin("Hierachy", 0, ImGuiUtils::k_FlagCommonWindow);
			ImGui::End();
		});
	}

	void RuntimeLayer::onDrawInspectorView(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<RenderTarget>& target, const LayoutRatio& layout, bool firstTime)
	{
		auto& param = m_layoutParam;
		context->submitRC([layout, firstTime, &context, &param]()
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
			ImGui::End();
		});
	}
}