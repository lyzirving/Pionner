#ifndef __PIONNER_ASSET_ASSIMP_ANIMATION_IMPORTER_H__
#define __PIONNER_ASSET_ASSIMP_ANIMATION_IMPORTER_H__

#include "core/Base.h"

struct aiScene;
struct aiAnimation;
struct aiNode;
struct aiNodeAnim;

namespace pio
{
	class Skeleton;
	class Animation;
	struct BoneInformation;

	class AssimpAnimationImporter
	{
	public:
		static Ref<Skeleton> ImportSkeleton(const aiScene *scene);
		static std::vector<Ref<Animation>> ImportAnimation(const aiScene *scene, Ref<Skeleton> &skeleton);

	private:
		static void TraverseNode(aiNode *node, Ref<Skeleton> &skeleton, const std::set<std::string> &boneNames);
		static void TraverseBone(aiNode *node, Ref<Skeleton> &skeleton, uint32_t parentBoneIndex);
		static void UpdateBoneOffsetMat(const aiScene *scene, Ref<Skeleton> &skeleton);

		static Ref<Animation> BuildAnimation(aiAnimation *anim, Ref<Skeleton> &skeleton);
	};
}

#endif