#ifndef __PIONNER_ANIMATION_SKELETON_H__
#define __PIONNER_ANIMATION_SKELETON_H__

#include "asset/Asset.h"

namespace pio
{
	class MeshSource;

	struct BoneInformation
	{
		// index in final Bone Matrices
		uint32_t Id{ ~0U };
		uint32_t ParentId{ ~0U };
		// transformation relative to the node's parent.
		glm::mat4 LocalTransform{ 1.f };
		// transformation from model space to bone space
		glm::mat4 OffsetMat{ 1.f };
	};

	class Skeleton
	{
	public:
		Skeleton() {}
		~Skeleton() = default;

		uint32_t updateBoneInformation(const std::string &name, uint32_t parentId, const glm::mat4 &localTransform);
		uint32_t updateBoneInformation(const std::string &name, glm::mat4 &offsetMat);

	public:
		inline const std::map<std::string, BoneInformation> &getBoneInformation() const { return m_boneInfos; }
		inline uint32_t getBoneInfoId(const std::string &name) { auto it = m_boneInfos.find(name); return (it != m_boneInfos.end()) ? it->second.Id : NullIndex; }

	private:
		std::map<std::string, BoneInformation> m_boneInfos{};
	};
}

#endif
