#include "MathDef.h"

namespace pio
{
	namespace Math
	{
		glm::vec3 Reminder(const glm::vec3& input, float reminder)
		{
			if(reminder < 0.f)
			{
				reminder = -reminder;
			}

			glm::vec3 result = input;
			glm::vec3 sign = glm::sign(result);

			auto calcReminder = [](float input, float sign, float reminder)
			{
				float val = input * sign;// val is >= 0.f
				if(val > reminder)
					val = val - reminder * int(val / reminder);
				return val * sign;
			};

			result.x = calcReminder(result.x, sign.x, reminder);
			result.y = calcReminder(result.y, sign.y, reminder);
			result.z = calcReminder(result.z, sign.z, reminder);

			return result;
		}

		float AngleCCW(const glm::vec3& start, const glm::vec3& end)
		{
			float dot = glm::dot(start, end);
			if(Equal(std::abs(dot), 1.f))
			{
				return dot > 0.f ? 0.f : 180.f;
			}			
			return glm::sign(glm::cross(start, end).z) * glm::degrees(std::acos(dot));
		}
	}

	namespace quaternion
	{
		bool Equal(const glm::quat& lhs, const glm::quat& rhs)
		{
			return Math::IsZero(glm::dot(lhs, rhs));
		}

		glm::quat Mix(const glm::quat& lhs, const glm::quat& rhs, float ratio)
		{
			float val = Math::Clamp(ratio, 0.f, 1.f);
			return glm::mix(lhs, rhs, val);
		}

		glm::quat RotationToQuat(const glm::vec3& start, const glm::vec3& dest)
		{
			glm::vec3 nStart = glm::normalize(start);
			glm::vec3 nDest = glm::normalize(dest);

			float cosTheta = glm::dot(nStart, nDest);
			glm::vec3 rotationAxis;

			if (cosTheta < -1 + 0.001f)
			{
				// special case when vectors in opposite directions :
				// there is no "ideal" rotation axis
				// So guess one; any will do as long as it's perpendicular to start
				// This implementation favors a rotation around the Up axis,
				// since it's often what you want to do.
				rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), nStart);
				if (glm::length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
					rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), nStart);

				rotationAxis = normalize(rotationAxis);
				return glm::angleAxis(glm::radians(180.0f), rotationAxis);
			}

			// Implementation from Stan Melax's Game Programming Gems 1 article
			rotationAxis = glm::cross(nStart, nDest);

			float s = glm::sqrt((1 + cosTheta) * 2);
			float invs = 1 / s;

			return glm::quat(
				s * 0.5f,
				rotationAxis.x * invs,
				rotationAxis.y * invs,
				rotationAxis.z * invs
			);
		}
	}
}