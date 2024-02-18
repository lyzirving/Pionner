#include "Animator.h"
#include "Skeleton.h"
#include "Animation.h"

#include "gfx/struct/Mesh.h"
#include "gfx/rhi/UniformBuffer.h"

#include "asset/AssetsManager.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Animator"

namespace pio
{
	Animator::Animator(const Ref<MeshSource> &meshSource) 
		: m_meshSource(meshSource), m_skeleton(meshSource->getSkeleton())
	{
		m_animations = m_meshSource->getAnimation();
		if (!m_animations.empty())
		{
			m_meshSource->m_boneTransform.obtainBlock();
			m_meshSource->m_boneTransformUB = UniformBuffer::Create(m_meshSource->m_boneTransform.Block.getByteUsed(),
																	(uint32_t)UBBindings::BoneTransform);
		}
	}

	Animator::~Animator()
	{
		m_meshSource.reset();
		m_skeleton.reset();
		m_animations.clear();
	}

	void Animator::play(uint32_t index)
	{
		PIO_ASSERT_RETURN(index < m_animations.size(), "play animation failed, index[%u] out of bounds[%u]", index, m_animations.size())
		if (m_runningAnimation != m_animations[index])
		{
			if (m_runningAnimation)
			{
				m_runningAnimation->pause();
			}
			m_runningAnimation = m_animations[index];
			m_runningAnimation->start();
			resetStatus();
		}
	}

	void Animator::stop()
	{
		if (m_runningAnimation)
			m_runningAnimation->stop();
		m_runningAnimation.reset();
	}

	void Animator::update(float deltaTimeSec)
	{
		PIO_ASSERT_RETURN(m_runningAnimation.use_count() != 0, "play() should be called");

		m_tick += (float)(m_runningAnimation->getTicksPerSec()) * deltaTimeSec;
		m_tick = fmod(m_tick, m_runningAnimation->getDuration());

		switch (m_runMode)
		{
			case AnimRunMode::CPU_Run:
			{
				CPUAnimUpdate();
				break;
			}
			case AnimRunMode::GPU_Run:
			{
				GPUAnimUpdate();
				break;
			}
			default:
				LOGE("invalid animatin mode[%u]", m_runMode);
				break;
		}
	}

	void Animator::CPUAnimUpdate()
	{
		const std::vector<MeshNode> &nodes = m_meshSource->getMeshNodes();
		const std::map<std::string, BoneInformation> &boneInfo = m_skeleton->getBoneInformation();
		glm::mat4 parentTransform{ 1.f };

		calculateBoneTransform(nodes, 0, boneInfo, parentTransform);
		m_meshSource->m_boneTransform.serialize();
		m_meshSource->m_boneTransform.GPUAnimated = false;
	}

	void Animator::GPUAnimUpdate()
	{
		GPUSkinning &gpu = m_runningAnimation->m_gpuSkinning;
		gpu.CurrentFrame = (m_tick / m_runningAnimation->getDuration()) * float(gpu.FrameCnt);

		m_meshSource->m_boneTransform.GPUTransform = gpu;
		m_meshSource->m_boneTransform.GPUAnimated = true;
	}

	void Animator::calculateBoneTransform(const std::vector<MeshNode> &nodes, uint32_t index,
										  const std::map<std::string, BoneInformation> &boneInfos, 
										  const glm::mat4 &parentTransform)
	{
		if (!m_runningAnimation)
			return;

		const MeshNode &curNode = nodes[index];
		std::string nodeName = curNode.Name;
		glm::mat4 nodeTransform = curNode.LocalTransform;

		Ref<Bone> bone = m_runningAnimation->findBone(nodeName);
		if (bone)
		{
			bone->update(m_tick);
			nodeTransform = bone->getLocalTransform();
		}
		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		auto it = boneInfos.find(nodeName);
		if (it != boneInfos.end())
		{
			const BoneInformation &info = it->second;
			if (info.Id != NullIndex && info.Id < PIO_MAX_BONE_MATRIX)
			{
				m_meshSource->m_boneTransform[info.Id] = globalTransformation * info.OffsetMat;
			}
		}

		for (uint32_t i = 0; i < curNode.Children.size(); i++)
		{
			calculateBoneTransform(nodes, curNode.Children[i], boneInfos, globalTransformation);
		}
	}

	void Animator::resetStatus()
	{
		m_tick = 0;
		m_meshSource->m_boneTransform.setDefault();
	}
}