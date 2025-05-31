#include "WireframeNode.h"

#include "gfx/renderer/RenderContext.h"

#include "component/StaticMeshComponent.h"

#include "asset/material/TexturedMaterial.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "WireframeNode"

namespace pio
{
	void WireframeNode::OnInit()
	{
		SetShowOutliner(false);
		auto comp = AddComponent<StaticMeshComponent>();		
	}

	void WireframeNode::SetColor(const glm::vec3& color)
	{
		const auto& elems = GetComponent<StaticMeshComponent>()->GetMaterialElems();
		if(elems.empty())
		{
			LOGE("none material elements");
			return;
		}

		if(elems[0]->Is<TexturedMaterial>())
		{
			elems[0]->As<TexturedMaterial>()->SetColor(color);
		}
	}
}