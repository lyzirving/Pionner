#pragma once

#include "GlobalSettings.h"

namespace pio
{
	class RenderContext;
	class CameraNode;
	class CameraNode;
	class RenderPass;

	class Renderer : public Object
	{
		RTTR_ENABLE(Object)
	public:
		Renderer() {}
		virtual ~Renderer() = default;

		virtual void OnAttach(const Ref<RenderContext>& context) = 0;
		virtual void OnDetach(const Ref<RenderContext>& context) = 0;
		virtual void OnExecute(const Ref<RenderContext> &context, const Ref<CameraNode>& camNode) = 0;

	protected:
		virtual void OnSetUp() = 0;

	public:
		static Ref<Renderer> Create(const RenderConfig &config);
	};
}