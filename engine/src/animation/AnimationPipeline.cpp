#include "AnimationPipeline.h"

#include "component/SkeletalMeshComponent.h"

#include "asset/mesh/SkinnedMesh.h"
#include "asset/mesh/AnimationClip.h"
#include "asset/mesh/Skeleton.h"

#include "scene/node/SkinnedMeshNode.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AnimationPipeline"

namespace pio
{
	void AnimationPipeline::Tick(const Ref<RenderContext>& context, std::map<uint32_t, WeakRef<SkeletalMeshComponent>>& comps)
	{
		auto it = comps.begin();
		while(it != comps.end())
		{
			if(it->second.expired())
			{
				it = comps.erase(it);
				continue;
			}
			auto aComp = it->second.lock();
			TickAComp(aComp);

			if(!aComp->GetAnimRegisterd())
			{
				// erase from map when animation ends
				it = comps.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void AnimationPipeline::TickAComp(const Ref<SkeletalMeshComponent>& comp)
	{
		TickAnimState(comp);

		if(comp->IsStart())
		{			
			if(comp->IsAnimSet())
			{
				TickBoneTransform(comp, comp->GetLocalTimePos());
				comp->OnBoneTransformChange();
			}			
		}
		else
		{
			// animation ends
			comp->SetAnimRegisterd(false);
		}
	}

	void AnimationPipeline::TickAnimState(const Ref<SkeletalMeshComponent>& comp)
	{
		if(!comp->GetAnimRegisterd())
		{
			comp->Stop();
			return;
		}

		if(comp->IsStart())
		{
			uint64_t interval = comp->GetInterval() + Time::DeltaTimeMs();
			uint64_t duration = comp->GetDuration() * 1000.f;
			if(interval <= duration)
			{
				comp->SetLocalTimePos(float(interval) / float(duration));
				comp->SetInterval(interval);
			}
			else if(comp->IsLoop())
			{
				interval = interval % duration;
				comp->SetLocalTimePos(float(interval) / float(duration));
				comp->SetInterval(interval);
			}
			else
			{
				comp->Stop();
			}
		}
	}

	void AnimationPipeline::TickBoneTransform(const Ref<SkeletalMeshComponent>& comp, float ratio)
	{
		auto clip = comp->GetAnimClip();
		if(clip)
		{
			float tick = Math::Clamp(ratio, 0.f, 1.f) * float(comp->GetTickNum());
			TickBoneTransform_Impl(tick, glm::mat4(1.f),
								   comp->GetMesh()->GetNodeRoot(),
								   comp->GetMesh()->GetSkeleton(),
								   comp->GetAnimClip(), 
								   comp->GetBoneTransform());			
		}		
	}

	void AnimationPipeline::TickReferenceBoneTransform(const Ref<SkeletalMeshComponent>& comp, /*out*/std::vector<glm::mat4>& boneTransforms)
	{
		Ref<AnimationClip> nullAnim;
		uint32_t jointSize = comp->GetMesh()->GetSkeleton()->GetJointSize();
		if(boneTransforms.size() != jointSize)
		{
			boneTransforms.clear();
			boneTransforms.resize(jointSize, glm::mat4(1.f));
		}
		TickBoneTransform_Impl(0.f, glm::mat4(1.f),
							   comp->GetMesh()->GetNodeRoot(),
							   comp->GetMesh()->GetSkeleton(),
							   nullAnim, boneTransforms);
	}

	void AnimationPipeline::TickBoneTransform_Impl(float fraction, const glm::mat4& parentTransform,
												   const SubMeshNode& node, const Ref<Skeleton>& skeleton,
												   const Ref<AnimationClip>& animClip,
												   std::vector<glm::mat4>& boneTransform)
	{
		glm::mat4 localTransform = node.LocalTransform;
		// if animClip is nullptr, we only get transform in local space
		if(animClip && animClip->HasJointSample(node.Name))
		{
			const auto& sample = animClip->GetJointSample(node.Name);
			localTransform = JointInterpolate::GetTranslate(sample, fraction) *
				JointInterpolate::GetRotation(sample, fraction) *
				JointInterpolate::GetScale(sample, fraction);
		}
		glm::mat4 globalTransform = parentTransform * localTransform;
		int32_t idx = skeleton->GetJointIndex(node.Name);
		if(idx >= 0)
		{
			boneTransform[idx] = globalTransform * skeleton->GetJoint(idx).InvBindPose;
		}

		for(size_t i = 0; i < node.Children.size(); ++i)
		{
			TickBoneTransform_Impl(fraction, globalTransform, node.Children[i], skeleton, animClip, boneTransform);
		}
	}
}