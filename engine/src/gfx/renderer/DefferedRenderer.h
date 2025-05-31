#pragma once

#include "Renderer.h"

namespace pio
{
	enum RenderBlockFlags;

	class DefferedRenderer : public Renderer
	{
		RTTR_ENABLE(Renderer)
	public:
		DefferedRenderer();
		~DefferedRenderer() = default;

		virtual void OnAttach(const Ref<RenderContext>& context) override;
		virtual void OnDetach(const Ref<RenderContext>& context) override;
		virtual void OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camNode) override;

	protected:
		virtual void OnSetUp() override;
		void ExecuteBlock(RenderBlockFlags flag, const std::vector<Ref<RenderPass>>& queue, const Ref<RenderContext>& context, const Ref<CameraNode>& camNode);

	private:
		std::unordered_map<std::string, Ref<RenderPass>> m_PassMap;
		std::vector<Ref<RenderPass>> m_ActiveQueue;
	};
}