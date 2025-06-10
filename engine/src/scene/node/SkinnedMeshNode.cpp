#include "SkinnedMeshNode.h"

#include "gfx/renderer/MeshRenderBuffer.h"
#include "gfx/renderer/RenderContext.h"

#include "component/TransformComponent.h"
#include "component/SkeletalMeshComponent.h"

#include "asset/mesh/SkinnedMesh.h"
#include "asset/material/Material.h"

#include "scene/Scene.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SkinnedMeshNode"

namespace pio
{
	SkinnedMeshNode::SkinnedMeshNode() : Node()
	{
	}

	void SkinnedMeshNode::SetMesh(const Ref<SkinnedMesh>& mesh)
	{
		GetComponent<SkeletalMeshComponent>()->SetMesh(mesh);
	}

	void SkinnedMeshNode::SetAnimationClip(uint32_t index)
	{
		GetComponent<SkeletalMeshComponent>()->SetAnimationClip(index);
	}

	void SkinnedMeshNode::StartAnimation()
	{
		GetComponent<SkeletalMeshComponent>()->Start();
	}

	void SkinnedMeshNode::StopAnimation()
	{
		GetComponent<SkeletalMeshComponent>()->Stop();
	}

	void SkinnedMeshNode::SetLoop(bool loop)
	{
		GetComponent<SkeletalMeshComponent>()->SetLoop(loop);
	}

	void SkinnedMeshNode::OnInit()
	{
		auto comp = AddComponent<SkeletalMeshComponent>();
		comp->SetTransformComponent(GetComponent<TransformComponent>());
	}

	void SkinnedMeshNode::OnBeginFrameTick(const Ref<RenderContext>& context)
	{
		m_BoundingBox.Reset();
	}

	void SkinnedMeshNode::OnTickComponents(const Ref<RenderContext>& context)
	{
		GetComponent<TransformComponent>()->OnTick();
		GetComponent<SkeletalMeshComponent>()->OnTick();
	}

	void SkinnedMeshNode::OnTick(const Ref<RenderContext>& context)
	{
		auto scene = m_Scene.lock();
		if(scene && (GetMobility() != Mobility_Ignore))
		{
			auto& box = scene->GetBoundingBox();
			box.Union(m_BoundingBox);
		}
	}

	void SkinnedMeshNode::OnRender(const Ref<RenderContext>& context)
	{
		auto& data = context->GetRenderingData();
		auto meshComp = GetComponent<SkeletalMeshComponent>();
		meshComp->OnRender(data);
	}
}