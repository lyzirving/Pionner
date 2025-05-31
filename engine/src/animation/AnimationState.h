#pragma once

#include "Common.h"

namespace pio
{
	class AnimationClip;

	class AnimationState
	{
	public:
		AnimationState() {}
		~AnimationState() = default;

	private:
		WeakRef<AnimationClip> m_Clip;
		float m_LocalTimePos{ 0.f };
		float m_BlendWeight{ 1.f };
		bool m_Enable{ false };
		bool m_bLoop{ false };
		uint64_t m_StartTime{ 0 };
		uint64_t m_Interval{ 0 };

		friend class SkeletalMeshComponent;
	};
}