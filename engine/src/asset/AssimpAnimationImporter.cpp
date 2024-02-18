#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include "AssimpAnimationImporter.h"
#include "AssimpUtils.h"

#include "gfx/struct/Mesh.h"

#include "animation/Animation.h"
#include "animation/Skeleton.h"
#include "animation/Bone.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AssimpAnimationImporter"

namespace pio
{
	static std::vector<ScaleKey> SplitScales(aiNodeAnim *channel)
	{
		std::vector<ScaleKey> scaleKeys{};
		for (int i = 0; i < channel->mNumScalingKeys; ++i)
		{
			aiVector3D scale = channel->mScalingKeys[i].mValue;
			float timeStamp = channel->mScalingKeys[i].mTime;
			scaleKeys.emplace_back(timeStamp, AssimpUtils::Vec3FromAiVector3D(scale));
		}
		return scaleKeys;
	}

	static std::vector<RotationKey> SplitRotations(aiNodeAnim *channel)
	{
		std::vector<RotationKey> rotationKeys{};
		for (int i = 0; i < channel->mNumRotationKeys; ++i)
		{
			aiQuaternion aiOrientation = channel->mRotationKeys[i].mValue;
			float timeStamp = channel->mRotationKeys[i].mTime;
			rotationKeys.emplace_back(timeStamp, AssimpUtils::QuatFromAiQuat(aiOrientation));
		}
		return rotationKeys;
	}

	static std::vector<TranslationKey> SplitTranslations(aiNodeAnim *channel)
	{
		std::vector<TranslationKey> translationKeys;
		for (int i = 0; i < channel->mNumPositionKeys; ++i)
		{
			aiVector3D aiPosition = channel->mPositionKeys[i].mValue;
			float timeStamp = channel->mPositionKeys[i].mTime;
			translationKeys.emplace_back(timeStamp, AssimpUtils::Vec3FromAiVector3D(aiPosition));
		}
		return translationKeys;
	}

	Ref<Skeleton> AssimpAnimationImporter::ImportSkeleton(const aiScene *scene)
	{
		Ref<Skeleton> skeleton;
		if (!scene)
			return skeleton;

		// 1. get unique bone names for hierarchy building
		std::set<std::string> boneNames;
		for (uint32_t i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh *mesh = scene->mMeshes[i];
			for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
			{
				boneNames.emplace(mesh->mBones[boneIndex]->mName.C_Str());
			}
		}

		if (boneNames.empty())
			return skeleton;

		skeleton = CreateRef<Skeleton>();
		// 2. build bone hierarchy by iterating the scene node
		TraverseNode(scene->mRootNode, skeleton, boneNames);
		// 3. update offset matrix
		UpdateBoneOffsetMat(scene, skeleton);
		
		return skeleton;
	}

	std::vector<Ref<Animation>> AssimpAnimationImporter::ImportAnimation(const aiScene *scene, Ref<Skeleton> &skeleton)
	{
		std::vector<Ref<Animation>> animations;
		if (!scene || !skeleton)
			return animations;

		uint32_t animCnt = scene->mNumAnimations;
		if (animCnt < 1)
			return animations;

		for (uint32_t i = 0; i < animCnt; i++)
		{
			aiAnimation *aiAnim = scene->mAnimations[i];
			Ref<Animation> anim = BuildAnimation(aiAnim, skeleton);
			if (anim)
				animations.push_back(anim);
		}
		return animations;
	}

	void AssimpAnimationImporter::TraverseNode(aiNode *node, Ref<Skeleton> &skeleton, const std::set<std::string> &boneNames)
	{
		if (boneNames.find(node->mName.C_Str()) != boneNames.end())
		{
			TraverseBone(node, skeleton, NullIndex);
		}
		else
		{
			for (uint32_t nodeIndex = 0; nodeIndex < node->mNumChildren; ++nodeIndex)
			{
				TraverseNode(node->mChildren[nodeIndex], skeleton, boneNames);
			}
		}
	}

	void AssimpAnimationImporter::TraverseBone(aiNode *node, Ref<Skeleton> &skeleton, uint32_t parentBoneIndex)
	{
		uint32_t boneIndex = skeleton->updateBoneInformation(node->mName.C_Str(), parentBoneIndex, 
													         AssimpUtils::Mat4FromAiMatrix4x4(node->mTransformation));
		for (uint32_t nodeIndex = 0; nodeIndex < node->mNumChildren; ++nodeIndex)
		{
			TraverseBone(node->mChildren[nodeIndex], skeleton, boneIndex);
		}
	}

	void AssimpAnimationImporter::UpdateBoneOffsetMat(const aiScene *scene, Ref<Skeleton> &skeleton)
	{
		if (!scene || !skeleton)
			return;

		for (uint32_t i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh *mesh = scene->mMeshes[i];
			for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
			{
				aiBone *bone = mesh->mBones[boneIndex];
				// insert or update bone's offset matrix
				skeleton->updateBoneInformation(bone->mName.C_Str(), AssimpUtils::Mat4FromAiMatrix4x4(bone->mOffsetMatrix));
			}
		}
	}

	Ref<Animation> AssimpAnimationImporter::BuildAnimation(aiAnimation *anim, Ref<Skeleton> &skeleton)
	{
		std::string animName = anim->mName.C_Str();
		Ref<Animation> result = CreateRef<Animation>(animName);
		result->m_duration = anim->mDuration;
		result->m_ticksPerSecond = anim->mTicksPerSecond;
		for (int32_t i = 0; i < anim->mNumChannels; i++)
		{
			auto *channel = anim->mChannels[i];
			std::string boneName = channel->mNodeName.data;
			uint32_t id = skeleton->getBoneInfoId(boneName);
			if (id == NullIndex)
			{
				LOGD("bone[%s] for animation[%s] does not exist, add it", boneName.c_str(), animName.c_str());
				id = skeleton->updateBoneInformation(boneName, glm::mat4(1.f));
			}
			auto bone = CreateRef<Bone>(boneName, id, SplitScales(channel), SplitRotations(channel), SplitTranslations(channel));
			result->insertBone(bone);
		}
		return result;
	}
}