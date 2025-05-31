#pragma once

#include "Common.h"

namespace pio
{
	class RenderContext;
	class SkeletalMeshComponent;
	class Skeleton;
	class AnimationClip;	
	class AnimationState;
	struct SubMeshNode;

	class AnimationPipeline
	{
	public:
		static void Tick(const Ref<RenderContext>& context, std::map<uint32_t, WeakRef<SkeletalMeshComponent>>& comps);
		static void TickAComp(const Ref<SkeletalMeshComponent>& comp);
		/*
		* @brief calculate bone transform for each joint
		* @param comp: the SkeletalMeshComponent that needs to be applied
		* @param ratio: animation ratio, which will be clamped into [0, 1]		
		*/
		static void TickBoneTransform(const Ref<SkeletalMeshComponent>& comp, float ratio);
		static void TickReferenceBoneTransform(const Ref<SkeletalMeshComponent>& comp, /*out*/std::vector<glm::mat4>& boneTransforms);
	private:
		static void TickAnimState(const Ref<SkeletalMeshComponent>& comp);
		static void TickBoneTransform_Impl(float fraction, const glm::mat4& parentTransform,
										   const SubMeshNode& node, const Ref<Skeleton>& skeleton,
										   const Ref<AnimationClip>& animClip,
										   /*out*/std::vector<glm::mat4>& boneTransform);
	};
}