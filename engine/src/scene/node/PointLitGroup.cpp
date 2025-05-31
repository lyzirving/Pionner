#include "PointLitGroup.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/renderer/PointLitShadowMap.h"

#include "gfx/rhi/UniformBuffer.h"

#include "component/PointLitGroupComponent.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PointLitGroup"

namespace pio
{	
	void PointLitGroup::OnInit()
	{
		AddComponent<PointLitGroupComponent>();
	}

	void PointLitGroup::OnRender(const Ref<RenderContext>& context)
	{
		auto comp = GetComponent<PointLitGroupComponent>();
		auto buffer = comp->GetUBuffer();		

		auto& data = context->GetRenderingData();
		data.UBuffs[buffer->Binding()] = buffer;
		data.Pipeline.PointLitShadowMap = comp->GetShadowMap();
	}

	void PointLitGroup::OnGroupSizeChanged()
	{
		GetComponent<PointLitGroupComponent>()->SetNum(m_Children.size());
	}
}