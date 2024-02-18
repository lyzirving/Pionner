#include "Skeleton.h"

#include "gfx/struct/Mesh.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Skeleton"

namespace pio
{
	uint32_t Skeleton::updateBoneInformation(const std::string &name, uint32_t parentId, const glm::mat4 &localTransform)
	{
		uint32_t id = m_boneInfos.size();
		auto it = m_boneInfos.find(name);
		if (it != m_boneInfos.end())
		{
			it->second.ParentId = parentId;
			it->second.LocalTransform = localTransform;
			id = it->second.Id;
		}
		else
		{
			BoneInformation info{};
			info.Id = id;
			info.ParentId = parentId;
			info.LocalTransform = localTransform;
			m_boneInfos.emplace(std::make_pair(name, info));
		}
		return id;
	}

	uint32_t Skeleton::updateBoneInformation(const std::string &name, glm::mat4 &offsetMat)
	{
		uint32_t id = m_boneInfos.size();
		auto it = m_boneInfos.find(name);
		if (it != m_boneInfos.end())
		{
			it->second.OffsetMat = offsetMat;
			id = it->second.Id;
		}
		else
		{
			BoneInformation info{};
			info.Id = id;
			info.OffsetMat = offsetMat;
			m_boneInfos.emplace(std::make_pair(name, info));
		}
		return id;
	}
}