#pragma once

#include "LitNode.h"
#include "gfx/GfxDef.h"

namespace pio
{
	class IndirectLightNode : public LitNode
	{
		RTTR_ENABLE(LitNode)
	public:
		IndirectLightNode() : LitNode() {}
		virtual ~IndirectLightNode() = default;

	protected:
		virtual void OnInit() override;
		virtual void OnTickComponents(const Ref<RenderContext>& context) override;
		virtual void OnRender(const Ref<RenderContext>& context) override;
	};
}