#pragma once

#include "StaticMesh.h"

namespace pio
{
	class Skeleton;
	class AnimationClip;

	class SkinnedMesh : public StaticMesh
	{
		RTTR_ENABLE(StaticMesh)
	public:
		SkinnedMesh(const ImportParams& params);

		virtual Ref<Asset> SpawnChild() override;
		virtual bool IsSkinned() const { return true; }

		const std::vector<std::vector<BoneInfluence>>& GetBoneInfluence() const { return m_BoneInfluences; }
		const Ref<Skeleton>& GetSkeleton() const { return m_Skeleton; }
		const std::vector<Ref<AnimationClip>>& GetAnimationClips() const { return m_AnimationClips; }

	private:
		Ref<Skeleton> m_Skeleton;		
		// first dimen is SubMesh's index, second dimen is vertex's index
		std::vector<std::vector<BoneInfluence>> m_BoneInfluences;
		std::vector<Ref<AnimationClip>> m_AnimationClips;

		friend class MeshImporter;
		friend class SkeletalMeshComponent;
	};
}