#ifndef __PIONNER_ANIMATION_BONE_H__
#define __PIONNER_ANIMATION_BONE_H__

#include "AnimDef.h"

namespace pio
{
	using TranslationKey = AnimationKey<glm::vec3>;
	using RotationKey = AnimationKey<glm::quat>;
	using ScaleKey = AnimationKey<glm::vec3>;

	class Bone
	{
	public:
		Bone(const std::string &name, uint32_t id,
			 std::vector<ScaleKey> &&scales,
			 std::vector<RotationKey> &&rotations,
			 std::vector<TranslationKey> &&translations);
		~Bone();

		void update(float animTime);

	public:
		inline uint32_t getId() const { return m_id; }
		inline const std::string &getName() const { return m_name; }
		inline const glm::mat4 &getLocalTransform() { return m_localTransform; }

	private:
		glm::mat4 interpolateTranslation(float animTime);
		glm::mat4 interpolateRotation(float animTime);
		glm::mat4 interpolateScale(float animTime);

		int getTranslationIndex(float animTime);
		int getRotationIndex(float animTime);
		int getScaleIndex(float animTime);

		inline float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) { return (animationTime - lastTimeStamp) / (nextTimeStamp - lastTimeStamp); }

	private:
		std::string m_name{};
		uint32_t m_id{ NullIndex };
		int32_t m_numTranslations{ 0 }, m_numRotations{ 0 }, m_numScalings{ 0 };
		std::vector<ScaleKey> m_scales{};
		std::vector<RotationKey> m_rotations{};
		std::vector<TranslationKey> m_translations{};
		glm::mat4 m_localTransform{ 1.f };
	};
}

#endif