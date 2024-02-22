#include "Animation.h"
#include "Skeleton.h"

#include "Animator.h"

#include "gfx/struct/Mesh.h"
#include "gfx/rhi/Texture.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Animation"

namespace pio
{
	Animation::~Animation()
	{
		auto it = m_bones.begin();
		while (it != m_bones.end())
		{
			it->second.reset();
			it = m_bones.erase(it);
		}
	}

	void Animation::buildGPUSkinning(Ref<Skeleton> &skeleton, const std::vector<MeshNode> &hierarchy)
	{
		PIO_ASSERT_RETURN(skeleton.use_count() != 0, "skeleton is invalid");
		const std::map<std::string, BoneInformation> &boneInfo = skeleton->getBoneInformation();

		PIO_ASSERT_RETURN(!boneInfo.empty(), "bone information is empty");
		PIO_ASSERT_RETURN((!Math::IsZero(m_duration) && m_ticksPerSecond != 0), "invalid animation attribute, duration[%f], tick per seconds[%d]", m_duration, m_ticksPerSecond);

		float interval = 1.0 / float(GPU_SKINNING_FPS);
		float curTick{ 0.f };
		uint32_t frameCnt{ 0 };

		uint64_t start = TimeUtil::CurrentTimeMs();

		while (curTick < m_duration)
		{
			FrameMats frameMats = FrameMats(boneInfo.size(), glm::mat4(1.f));
			glm::mat4 parentMat{ 1.f };
			calcGPUSkinning(boneInfo, hierarchy, 0, parentMat, curTick, frameMats);

			m_gpuSkinning.Mats.push_back(std::move(frameMats));
			frameCnt++;
			curTick += m_ticksPerSecond * interval;
		}
		m_gpuSkinning.FrameCnt = frameCnt;
		m_gpuSkinning.BoneNum = boneInfo.size();
		
		PIO_ASSERT_RETURN((m_gpuSkinning.BoneNum != 0 && m_gpuSkinning.FrameCnt != 0),
						  "invalid GPU skinning bone num[%u] or frame cnt[%u]", 
						  m_gpuSkinning.BoneNum, m_gpuSkinning.FrameCnt);
		PIO_ASSERT_RETURN(m_gpuSkinning.Mats.size() == m_gpuSkinning.FrameCnt, "fail to make gpu skinning transformation");

		/*
		* For bone mat, each mat's item num is 3 * 4, 
		* We ignore the last row for every 4x4 mat to save capacity,
		* Because the last row is always [0 0 0 1]
		*/
		const uint32_t cnt = 12 * m_gpuSkinning.BoneNum * m_gpuSkinning.FrameCnt;
		const uint32_t byteSize = cnt * sizeof(float);
		void *data = PIO_MALLOC(byteSize);
		float *pData = static_cast<float *>(data);
		for (uint32_t i = 0; i < m_gpuSkinning.FrameCnt; i++)
		{
			auto &frameMats = m_gpuSkinning.Mats[i];
			uint32_t o1 = i * m_gpuSkinning.BoneNum * 12;
			for (uint32_t j = 0; j < m_gpuSkinning.BoneNum; j++)
			{
				auto &boneMat = frameMats[j];
				uint32_t o2 = o1 + j * 12;
				// the array is organized in column-order
				// a texel stores a matrix's column 
				pData[o2 + 0] = boneMat[0].x;
				pData[o2 + 1] = boneMat[0].y;
				pData[o2 + 2] = boneMat[0].z;

				pData[o2 + 3] = boneMat[1].x;
				pData[o2 + 4] = boneMat[1].y;
				pData[o2 + 5] = boneMat[1].z;

				pData[o2 + 6] = boneMat[2].x;
				pData[o2 + 7] = boneMat[2].y;
				pData[o2 + 8] = boneMat[2].z;

				pData[o2 + 9] = boneMat[3].x;
				pData[o2 + 10] = boneMat[3].y;
				pData[o2 + 11] = boneMat[3].z;
			}
		}
		m_gpuSkinning.BufferTexture = BufferTexture::Create(data, byteSize, ImageInternalFormat::RGB32F);

		uint64_t end = TimeUtil::CurrentTimeMs();
		LOGD("parse animation[%s]'s GPU skinning data, take time[%lu]ms", m_name.c_str(), (end - start));
	}

	Ref<Bone> Animation::findBone(const std::string &name)
	{
		Ref<Bone> bone;
		auto it = m_bones.find(name);
		if (it != m_bones.end())
		{
			bone = it->second;
		}
		return bone;
	}

	void Animation::start()
	{
		m_gpuSkinning.CurrentFrame = 0;
	}

	void Animation::pause()
	{
	}

	void Animation::stop()
	{
		m_gpuSkinning.CurrentFrame = 0;
	}

	void Animation::calcGPUSkinning(const std::map<std::string, BoneInformation> &boneInfo, const std::vector<MeshNode> &hierarchy, uint32_t index, glm::mat4 &parentTransform, float curTick, FrameMats &frameMats)
	{
		PIO_ASSERT_RETURN(index < hierarchy.size(), "invalid index[%u], hierarchy size[%u]", index, hierarchy.size());
		const MeshNode &node = hierarchy[index];
		std::string nodeName = node.Name;
		glm::mat4 nodeTransform = node.LocalTransform;
		Ref<Bone> bone = findBone(nodeName);
		if (bone)
		{
			bone->update(curTick);
			nodeTransform = bone->getLocalTransform();
		}
		glm::mat4 globalTransformation = parentTransform * nodeTransform;
		auto it = boneInfo.find(nodeName);
		if (it != boneInfo.end())
		{
			const BoneInformation &info = it->second;
			if (info.Id != NullIndex && info.Id < boneInfo.size())
			{
				frameMats[info.Id] = globalTransformation * info.OffsetMat;
			}
		}

		for (uint32_t i = 0; i < node.Children.size(); i++)
			calcGPUSkinning(boneInfo, hierarchy, node.Children[i], globalTransformation, curTick, frameMats);
	}

	AnimationAsset::AnimationAsset(const Ref<MeshSource> &meshSource)
		: Asset(), m_animator(CreateRef<Animator>(meshSource))
	{
	}

	AnimationAsset::~AnimationAsset()
	{
		m_animator.reset();
	}
}