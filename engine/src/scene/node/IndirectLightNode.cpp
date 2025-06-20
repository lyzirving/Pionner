#include "IndirectLightNode.h"

#include "gfx/renderer/IndirectLight.h"
#include "gfx/renderer/RenderContext.h"

#include "component/IndirectLightComponent.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "IndirectLightNode"

namespace pio
{
	void IndirectLightNode::OnInit()
	{
		AddComponent<IndirectLightComponent>();
	}

	void IndirectLightNode::OnTickComponents(const Ref<RenderContext>& context)
	{
		GetComponent<IndirectLightComponent>()->OnTick();
	}

	void IndirectLightNode::OnRender(const Ref<RenderContext>& context)
	{
		auto& data = context->GetRenderingData();
		data.PermutationFlags |= (uint32_t)ShaderPermutationFlag::USE_INDIRECT_LIGHT;
		data.Pipeline.IndirectLight = GetComponent<IndirectLightComponent>()->GetIndirectLight();
	}
}