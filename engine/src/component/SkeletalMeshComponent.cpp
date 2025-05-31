#include "SkeletalMeshComponent.h"
#include "TransformComponent.h"

#include "asset/mesh/SkinnedMesh.h""
#include "asset/mesh/Skeleton.h"
#include "asset/mesh/AnimationClip.h"
#include "asset/material/Material.h"

#include "gfx/renderer/MeshRenderBuffer.h"
#include "gfx/renderer/UMotionBuffer.h"
#include "gfx/renderer/RenderContext.h"

#include "scene/node/SkinnedMeshNode.h"

#include "animation/AnimationSystem.h"
#include "animation/AnimationPipeline.h"
#include "animation/AnimationState.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SkeletalMeshComponent"

namespace pio
{
	void SkeletalMeshComponent::OnMeshChange()
	{
		if(!m_Mesh)
		{
			m_ReferenceBoneTransforms.resize(PIO_MAX_BONE_MATRIX, glm::mat4(1.f));
			m_BoneTransforms.resize(PIO_MAX_BONE_MATRIX, glm::mat4(1.f));
			OnBoneTransformChange();
		}
		else
		{
			m_Mesh->OnInit();
			CreateBuffer();
			CreateSkinningBuffer();
			m_MaterialElems.assign(m_Mesh->m_Materials.begin(), m_Mesh->m_Materials.end());
			AnimationPipeline::TickReferenceBoneTransform(Self<SkeletalMeshComponent>(), m_ReferenceBoneTransforms);
			m_BoneTransforms.assign(m_ReferenceBoneTransforms.begin(), m_ReferenceBoneTransforms.end());
		}				
	}

	void SkeletalMeshComponent::OnbCastShadowChange()
	{
	}

	void SkeletalMeshComponent::OnAnimRegisterdChange()
	{
		if(!m_Mesh)
		{
			LOGW("skeletal hasn't been set yet");
			return;
		}

		if(m_AnimRegisterd)
		{
			AnimationSystem::Get()->Inject(Self<SkeletalMeshComponent>());
		}
	}

	SkeletalMeshComponent::SkeletalMeshComponent() : Component()
	{
		m_AnimationState = CreateRef<AnimationState>();
	}

	void SkeletalMeshComponent::OnTick()
	{	
		for(size_t i = 0; i < m_MotionBuffers.size(); ++i)
		{
			m_MotionBuffers[i]->As<UMotionBuffer>()->Upload();
		}

		if(m_SkinningUBlock && m_SkinningUBlock->AnyChange())
		{
			m_Context.lock()->UploadData(m_SkinningUBlock->GetBuffer()->As<void*>(), m_SkinningUBlock->GetByteUsed(), m_SkinningUBuffer);
			m_SkinningUBlock->Invalidate(false);
		}

		for(auto& elem : m_MaterialElems)
		{
			elem->OnAttrsChange();
		}
	}

	void SkeletalMeshComponent::OnTransformChange(const Ref<TransformComponent>& comp)
	{
		if(!m_Mesh)
			return;

		auto* node = m_Host.lock()->As<SkinnedMeshNode>();
		const auto& subMesh = m_Mesh->GetSubMesh();		
		for(size_t i = 0; i < subMesh.size(); i++)
		{
			const auto& mesh = subMesh[i];
			auto* buffer = m_MotionBuffers[i]->As<UMotionBuffer>();
			buffer->SetTransform(comp->GetMat());
			buffer->SetLocalTransform(mesh.Transform);
			buffer->SetSkeletalOn(IsStart());

			if(node->GetMobility() != Mobility_Ignore)
			{
				// bounding box can't consider skeletal animation
				node->m_BoundingBox.Union((comp->GetMat() * mesh.Transform) * mesh.BoundingBox);
			}
		}
	}

	void SkeletalMeshComponent::OnBoneTransformChange()
	{
		if(!m_SkinningUBlock)
		{
			return;
		}

		auto matArray = (*m_SkinningUBlock)["Transform"]->As<UniformMatArrayLayout>();
		for(size_t i = 0; i < m_BoneTransforms.size(); i++)
		{
			m_SkinningUBlock->WriteAt(glm::value_ptr(m_BoneTransforms[i]), sizeof(glm::mat4), (*matArray)[i].GetAlignOffset());
		}
		auto layout = (*m_SkinningUBlock)["Num"];
		int32_t num = m_BoneTransforms.size();
		m_SkinningUBlock->WriteAt(&num, sizeof(int32_t), layout->GetAlignOffset());
	}

	void SkeletalMeshComponent::OnRender(RenderingData& data)
	{
		if(!m_Mesh)
			return;

		const auto& subMesh = m_Mesh->GetSubMesh();
		const auto& materialList = m_Mesh->GetMaterial();
		for(size_t i = 0; i < subMesh.size(); i++)
		{
			const auto& mesh = subMesh[i];
			const auto& material = materialList[mesh.MaterialIdx];

			MeshRenderingItem item;
			item.Mode = material->IsTranslucency() ? RenderingMode_Transparent : RenderingMode_Opaque;
			item.DrawFlag = DrawMode_Triangle;
			item.MeshBuff = m_RenderBuff[i];
			item.Material = material;
			item.UniformBuffers.push_back(m_MotionBuffers[i]);

			if(m_SkinningUBuffer)
				item.UniformBuffers.push_back(m_SkinningUBuffer);

			if(material->IsTranslucency())
				data.TransparentMeshItems.push_back(item);
			else
				data.OpaqueMeshItems.push_back(item);

			if(material->IsCastShadow())
				data.ShadowCasterItems.push_back(item);			
		}		
	}

	void SkeletalMeshComponent::Start()
	{
		if(!IsAnimSet())
		{
			LOGE("animation hasn't been set");
			return;
		}
		// already started
		if(m_AnimationState->m_Enable)
		{
			return;
		}

		LOGD("animation[%s] start", GetAnimClip()->Name().c_str());
		m_AnimationState->m_Enable = true;
		SetStartTime(Time::CurrentTimeMs());

		if(!m_AnimRegisterd)
		{
			SetAnimRegisterd(true);
		}
	}

	void SkeletalMeshComponent::Stop()
	{
		if(m_AnimationState->m_Enable)
		{
			LOGD("animation[%s] stop", GetAnimClip()->Name().c_str());
			m_AnimationState->m_Enable = false;
			SetLocalTimePos(0.f);
			SetInterval(0);

			m_BoneTransforms.clear();
			m_BoneTransforms.assign(m_ReferenceBoneTransforms.begin(), m_ReferenceBoneTransforms.end());
			OnBoneTransformChange();

			SetAnimRegisterd(false);
		}
	}

	void SkeletalMeshComponent::SetStartTime(uint64_t time)
	{
		m_AnimationState->m_StartTime = time;
	}

	void SkeletalMeshComponent::SetInterval(uint64_t time)
	{
		m_AnimationState->m_Interval = time;
	}

	void SkeletalMeshComponent::SetLoop(bool loop)
	{
		m_AnimationState->m_bLoop = loop;
	}

	void SkeletalMeshComponent::SetAnimationClip(uint32_t index)
	{
		if(!m_Mesh)
		{
			LOGE("mesh is invalid");
			return;
		}

		const auto& clips = m_Mesh->GetAnimationClips();
		if(index < clips.size())
		{
			SetAnimationClip(clips[index]);
		}
		else
		{
			LOGE("index[%u] is out of bounds[%u]", index, clips.size());			
		}		
	}

	void SkeletalMeshComponent::SetAnimationClip(const Ref<AnimationClip>& clip)
	{
		m_AnimationState->m_Clip = clip;
	}

	void SkeletalMeshComponent::RemoveAnimationClip()
	{
		Stop();
		m_AnimationState->m_Clip.reset();
	}

	void SkeletalMeshComponent::SetLocalTimePos(float pos)
	{
		m_AnimationState->m_LocalTimePos = Math::Clamp(pos, 0.f, 1.f);
	}

	void SkeletalMeshComponent::SetBlendWeight(float weight)
	{
		m_AnimationState->m_BlendWeight = Math::Clamp(weight, 0.f, 1.f);
	}

	bool SkeletalMeshComponent::IsAnimSet() const
	{
		return !m_AnimationState->m_Clip.expired();
	}

	bool SkeletalMeshComponent::IsStart() const
	{
		return IsAnimSet() && m_AnimationState->m_Enable;
	}

	bool SkeletalMeshComponent::IsLoop() const
	{
		return m_AnimationState->m_bLoop;
	}

	float SkeletalMeshComponent::GetLocalTimePos() const
	{
		return m_AnimationState->m_LocalTimePos;
	}

	float SkeletalMeshComponent::GetBlendWeight() const
	{
		return m_AnimationState->m_BlendWeight;
	}

	uint64_t SkeletalMeshComponent::GetStartTime() const
	{
		return m_AnimationState->m_StartTime;
	}

	uint64_t SkeletalMeshComponent::GetInterval() const
	{
		return m_AnimationState->m_Interval;
	}

	float SkeletalMeshComponent::GetDuration() const
	{
		return GetAnimClip()->GetDuration();
	}

	uint32_t SkeletalMeshComponent::GetTickNum() const
	{
		return GetAnimClip()->GetTickNum();
	}

	Ref<AnimationClip> SkeletalMeshComponent::GetAnimClip() const
	{
		return m_AnimationState->m_Clip.lock();
	}

	void SkeletalMeshComponent::CreateBuffer()
	{
		if(!m_Mesh)
			return;

		auto context = m_Context.lock();
		if(!context)
		{
			LOGE("err! context is invalid");
			return;
		}

		const auto& subMesh = m_Mesh->GetSubMesh();
		const auto& boneInfluences = m_Mesh->GetBoneInfluence();

		m_RenderBuff.clear();
		m_MotionBuffers.clear();		

		m_RenderBuff.reserve(subMesh.size());
		m_MotionBuffers.reserve(subMesh.size());		
		for(size_t i = 0; i < subMesh.size(); i++)
		{
			const auto& mesh = subMesh[i];
			const auto& influence = boneInfluences[i];
			m_RenderBuff.emplace_back(CreateRef<MeshRenderBuffer>());
			m_RenderBuff[i]->Setup(context, mesh.Vertice, mesh.Indice);

			auto vbo = VertexBuffer::Create(context, influence.data(), influence.size() * sizeof(BoneInfluence));
			vbo->SetBufferLayout(VertexBuffer::To<BoneInfluence>());
			m_RenderBuff[i]->Vao->AddVertexBuffer(vbo);

			std::string name(mesh.Name);
			name.append("_UMotionBuffer");
			m_MotionBuffers.emplace_back(CreateRef<UMotionBuffer>(context, name));
		}
	}

	void SkeletalMeshComponent::CreateSkinningBuffer()
	{
		if(m_SkinningUBlock)
			return;

		m_SkinningUBlock = CreateRef<UniformBlock>();
		m_SkinningUBlock->PushBack("Transform", UniformBlock::CreateData(UniformType::Mat4Array, "Transform", PIO_MAX_BONE_MATRIX));
		m_SkinningUBlock->PushBack("Num", UniformBlock::CreateData(UniformType::Int, "Num"));
		m_SkinningUBlock->Calculate();

		m_SkinningUBuffer = UniformBuffer::Create(m_Context.lock(), m_SkinningUBlock->GetByteUsed(),
												  UBBinging_Skeleton, BufferUsage::Dynamic);		
	}
}