#pragma once

#include "asset/Asset.h"

namespace pio
{
	class Skeleton;

	class AnimationClip : public Asset
	{
		RTTR_ENABLE(Asset)
	public:
		AnimationClip(const ImportParams& params);

		float GetDuration() const { return float(m_FrameCount) / m_FramesPerSecond; }
		uint32_t GetTickNum() const { return m_FrameCount; }
		bool HasJointSample(const std::string& name) const { return m_Samples.find(name) != m_Samples.end(); }
		JointSample& GetJointSample(const std::string& name) { return m_Samples[name]; }

	private:
		std::unordered_map<std::string, JointSample> m_Samples;		
		uint32_t m_FrameCount{ 0 };
		float m_FramesPerSecond{ 0.f };
		bool m_bLoop{ false };

		friend class MeshImporter;
	};
}