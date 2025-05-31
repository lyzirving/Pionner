#pragma once

#include "Common.h"

namespace pio
{
    #define ROOT_JOINT_INDEX (255u)

	struct Joint
	{
		glm::mat4 InvBindPose{ 1.f };// transform joint from model local space to joint space
		std::string Name{};
		std::vector<uint8_t> Children;		
		uint8_t Parent{ ROOT_JOINT_INDEX };
		uint8_t Level{ 0 };

		bool IsRoot() const { return Parent == ROOT_JOINT_INDEX; }
		bool IsLeaf() const { return Children.empty(); }
	};

	template<typename T>
	struct AnimationKey
	{
		T Value;
		// 0.0f = beginning of animation clip, 1.0f = end of animation clip 
		float FrameTime;

		AnimationKey() = default;
		AnimationKey(const float frameTime, const T& value) 
			: FrameTime(frameTime), Value(value)
		{
		}
	};

	template<typename T>
	inline bool AnimationKeySort(const T& lhs, const T& rhs)
	{
		return lhs.FrameTime < rhs.FrameTime;
	}

	using TranslationKey = AnimationKey<glm::vec3>;
	using RotationKey = AnimationKey<glm::quat>;
	using ScaleKey = AnimationKey<glm::vec3>;

	struct JointSample
	{
		std::string Name;
		std::vector<TranslationKey> Position;
		std::vector<RotationKey> Rotation;
		std::vector<ScaleKey> Scale;
	};

	struct BoneInfluence
	{
		float Weights[PIO_MAX_BONE_INFLUENCE] = { 0.f, 0.f, 0.f, 0.f };
		int32_t Indices[PIO_MAX_BONE_INFLUENCE] = { -1, -1, -1, -1 };
	};

	namespace JointInterpolate
	{
		glm::mat4 GetTranslate(const JointSample& sample, float fraction);
		glm::mat4 GetRotation(const JointSample& sample, float fraction);
		glm::mat4 GetScale(const JointSample& sample, float fraction);

		int GetTranslateIndex(const JointSample& sample, float fraction);
		int GetRotationIndex(const JointSample& sample, float fraction);
		int GetScaleIndex(const JointSample& sample, float fraction);

		float GetFactor(float last, float next, float fraction);
	}
}