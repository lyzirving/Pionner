#ifndef __PIONNER_ANIMATION_ANIMATOR_H__
#define __PIONNER_ANIMATION_ANIMATOR_H__

#include "AnimDef.h"

namespace pio
{
	class Animation;
	class MeshSource;
	class Skeleton;
	struct MeshNode;
	struct BoneInformation;

	class Animator
	{
	public:
		Animator(const Ref<MeshSource> &meshSource);
		~Animator();

		void play(uint32_t index = 0);
		void stop();
		void update(float deltaTimeSec);

	public:
		inline void setRunMode(AnimRunMode mode) { m_runMode = mode; }
		inline bool isPlaying() const { return m_runningAnimation != nullptr; }
		inline const Ref<MeshSource> &getMeshSource() const { return m_meshSource; }

	private:
		void CPUAnimUpdate();
		void GPUAnimUpdate();

		void calculateBoneTransform(const std::vector<MeshNode> &nodes, uint32_t index, 
									const std::map<std::string, BoneInformation> &boneInfos,
									const glm::mat4 &parentTransform);
		void resetStatus();

	private:
		// Current animation time in tick
		float m_tick{ 0.f };
		Ref<MeshSource> m_meshSource;
		Ref<Skeleton> m_skeleton;
		Ref<Animation> m_runningAnimation;
		std::vector<Ref<Animation>> m_animations;
		AnimRunMode m_runMode{ AnimRunMode::CPU_Run };
	};
}

#endif
