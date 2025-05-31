#include "SkeletalDef.h"

namespace pio
{
	namespace JointInterpolate
	{
		glm::mat4 GetTranslate(const JointSample& sample, float fraction)
		{
			if(1 == sample.Position.size())
			{
				return glm::translate(glm::mat4(1.f), sample.Position[0].Value);
			}

			int index0 = GetTranslateIndex(sample, fraction);
			int index1 = index0 + 1;
			float factor = GetFactor(sample.Position[index0].FrameTime, 
									 sample.Position[index1].FrameTime,
									 fraction);

			glm::vec3 position = glm::mix(sample.Position[index0].Value,
										  sample.Position[index1].Value,
										  factor);

			return glm::translate(glm::mat4(1.f), position);
		}

		glm::mat4 GetRotation(const JointSample& sample, float fraction)
		{
			if(1 == sample.Rotation.size())
			{
				return glm::toMat4(glm::normalize(sample.Rotation[0].Value));
			}

			int index0 = GetRotationIndex(sample, fraction);
			int index1 = index0 + 1;
			float factor = GetFactor(sample.Rotation[index0].FrameTime,
									 sample.Rotation[index1].FrameTime,
									 fraction);
			glm::quat rotation = glm::slerp(sample.Rotation[index0].Value,
											sample.Rotation[index1].Value,
											factor);
			return glm::toMat4(glm::normalize(rotation));
		}

		glm::mat4 GetScale(const JointSample& sample, float fraction)
		{
			if(1 == sample.Scale.size())
				return glm::scale(glm::mat4(1.f), sample.Scale[0].Value);

			int index0 = GetScaleIndex(sample, fraction);
			int index1 = index0 + 1;
			float factor = GetFactor(sample.Scale[index0].FrameTime,
									 sample.Scale[index1].FrameTime,
									 fraction);

			glm::vec3 scale = glm::mix(sample.Scale[index0].Value,
									   sample.Scale[index1].Value,
									   factor);

			return glm::scale(glm::mat4(1.f), scale);
		}

		int GetTranslateIndex(const JointSample& sample, float fraction)
		{
			for(int index = 0; index < sample.Position.size() - 1; ++index)
			{
				if(fraction < sample.Position[index + 1].FrameTime)
					return index;
			}
			return 0;
		}

		int GetRotationIndex(const JointSample& sample, float fraction)
		{
			for(int index = 0; index < sample.Rotation.size() - 1; ++index)
			{
				if(fraction < sample.Rotation[index + 1].FrameTime)
					return index;
			}
			return 0;
		}

		int GetScaleIndex(const JointSample& sample, float fraction)
		{
			for(int index = 0; index < sample.Scale.size() - 1; ++index)
			{
				if(fraction < sample.Scale[index + 1].FrameTime)
					return index;
			}
			return 0;
		}

		float GetFactor(float last, float next, float fraction)
		{
			return (fraction - last) / (next - last);
		}
	}
}