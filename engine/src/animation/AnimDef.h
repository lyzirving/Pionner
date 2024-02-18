#ifndef __PIONNER_ANIMATION_ANIM_DEF_H__
#define __PIONNER_ANIMATION_ANIM_DEF_H__

#include "core/Base.h"

namespace pio
{
	#define GPU_SKINNING_FPS (60)

	class BufferTexture;

	using FrameMats = std::vector<glm::mat4>;
	using GPUSkinningMats = std::vector<FrameMats>;

	struct GPUSkinning
	{
		uint32_t FrameCnt{ 0 };
		uint32_t BoneNum{ 0 };
		GPUSkinningMats Mats{};
		Ref<BufferTexture> BufferTexture{};
		uint32_t CurrentFrame{ 0 };
	};

	enum class AnimRunMode : uint8_t
	{
		CPU_Run, GPU_Run
	};

	template<typename T>
	struct AnimationKey
	{
		T Value;
		// 0.0f = beginning of animation clip, 1.0f = end of animation clip 
		float FrameTime;

		AnimationKey() = default;
		AnimationKey(const float frameTime, const T &value) : FrameTime(frameTime), Value(value)
		{
		}
	};
}

#endif