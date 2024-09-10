#include "RuntimeLayer.h"
#include "ImGuiUtils.h"

#include "window/Window.h"
#include "asset/AssetMgr.h"

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

	void RuntimeLayer::onUpdate(Ref<RenderContext>& context, Ref<RenderTarget>& target)
	{		
		if(!target)
			return;

		auto uid = target->assetHnd();
		auto& param = m_layoutParam;
		auto& firstTime = m_firstTimeShow;
		context->submitRC([uid, &param, &firstTime]()
		{			
			auto t = AssetMgr::GetRuntimeAsset<RenderTarget>(uid);
			const auto& texture = t->frameBuffer()->colorBuffers()[0];
						
			if(firstTime)
			{
				auto* mainVp = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(mainVp->Pos);
				ImGui::SetNextWindowSize(ImVec2(param.Vp.w(), param.Vp.h()));
				firstTime = false;
			}	
			ImGuiUtils::SetWindowPadding(glm::vec2(0.f));
			ImGui::Begin("Scene", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
			ImGuiUtils::DrawImage(texture->id(), glm::vec2(param.Vp.w(), param.Vp.h()), glm::vec2(0.f, 1.f), glm::vec2(1.f, 0.f), 0.f, 0.f);
			ImGui::End();
			ImGuiUtils::ResetWindowPadding();
		});
	}
}