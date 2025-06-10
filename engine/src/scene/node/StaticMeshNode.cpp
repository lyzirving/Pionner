#include "StaticMeshNode.h"

#include "gfx/renderer/MeshRenderBuffer.h"
#include "gfx/renderer/RenderContext.h"

#include "component/TransformComponent.h"
#include "component/StaticMeshComponent.h"

#include "asset/mesh/StaticMesh.h"
#include "asset/material/Material.h"

#include "scene/Scene.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "StaticMeshNode"

namespace pio
{
	StaticMeshNode::StaticMeshNode() : Node()
	{
	}

	void StaticMeshNode::SetMesh(const Ref<StaticMesh>& mesh)
	{
		GetComponent<StaticMeshComponent>()->SetMesh(mesh);
	}

	void StaticMeshNode::SetSubMesh(int32_t index, const SubMesh& mesh)
	{
		GetComponent<StaticMeshComponent>()->SetSubMesh(index, mesh);
	}

	void StaticMeshNode::OnInit()
	{	
		auto comp = AddComponent<StaticMeshComponent>();
		comp->SetTransformComponent(GetComponent<TransformComponent>());
	}

	void StaticMeshNode::OnBeginFrameTick(const Ref<RenderContext>& context)
	{
		m_BoundingBox.Reset();
	}

	void StaticMeshNode::OnTickComponents(const Ref<RenderContext>& context)
	{
		GetComponent<TransformComponent>()->OnTick();
		GetComponent<StaticMeshComponent>()->OnTick();
	}

	void StaticMeshNode::OnTick(const Ref<RenderContext>& context)
	{
		auto scene = m_Scene.lock();
		if(scene && (GetMobility() != Mobility_Ignore))
		{
			auto& box = scene->GetBoundingBox();
			box.Union(m_BoundingBox);			
		}
	}

	void StaticMeshNode::OnRender(const Ref<RenderContext>& context)
	{
		auto& data = context->GetRenderingData();
		auto meshComp = GetComponent<StaticMeshComponent>();
		meshComp->OnRender(data);
	}	
}