#ifndef __PIONNER_GFX_RENDERER_RENDERER_H__
#define __PIONNER_GFX_RENDERER_RENDERER_H__

#include "GlobalSettings.h"

namespace pio
{
	class RenderContext;
	class CameraNode;
	class RenderPass;

	class Renderer : public std::enable_shared_from_this<Renderer>
	{
	public:
		Renderer() {}
		virtual ~Renderer() = default;

		virtual void onAttach(Ref<RenderContext>& context) = 0;
		virtual void onDetach(Ref<RenderContext>& context) = 0;
		virtual void onSetUp() = 0;
		virtual void onExecute(Ref<RenderContext> &context, Ref<CameraNode>& camNode) = 0;
		virtual Ref<RenderPass> getPass(uint32_t type) = 0;

		template<typename T>
		Ref<T> self() { return RefCast<Renderer, T>(shared_from_this()); }

	public:
		static Ref<Renderer> Create(const RenderConfig &config);
	};
}

#endif