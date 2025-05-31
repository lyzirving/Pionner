#pragma once

#include "Common.h"

namespace pio
{
	class CameraNode;
	class RenderContext;

	class RenderPipeline
	{
	public:
		RenderPipeline(const Ref<RenderContext>& context);
		~RenderPipeline() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void Render(const Ref<RenderContext>& context, const std::vector<Ref<CameraNode>>& camNodes);

	private:
		WeakRef<RenderContext> m_Context;
	};
}