#include "Bone.h"

namespace pio
{
	Bone::Bone(const std::string &name, uint32_t id, 
			   std::vector<ScaleKey> &&scales, 
			   std::vector<RotationKey> &&rotations, 
			   std::vector<TranslationKey> &&translations)
		: m_name(name), m_id(id), m_scales(std::move(scales)), m_rotations(std::move(rotations))
		, m_translations(std::move(translations))
	{
		m_numTranslations = m_translations.size();
		m_numRotations = m_rotations.size();
		m_numScalings = m_scales.size();
	}

	Bone::~Bone()
	{
		std::vector<ScaleKey>().swap(m_scales);
		std::vector<RotationKey>().swap(m_rotations);
		std::vector<TranslationKey>().swap(m_translations);
	}

	void Bone::update(float animTime)
	{
		glm::mat4 translation = interpolateTranslation(animTime);
		glm::mat4 rotation = interpolateRotation(animTime);
		glm::mat4 scale = interpolateScale(animTime);
		m_localTransform = translation * rotation * scale;
	}

	glm::mat4 Bone::interpolateTranslation(float animTime)
	{
		if (1 == m_numTranslations)
			return glm::translate(glm::mat4(1.0f), m_translations[0].Value);

		int p0Index = getTranslationIndex(animTime);
		int p1Index = p0Index + 1;
		float scaleFactor = getScaleFactor(m_translations[p0Index].FrameTime,
										   m_translations[p1Index].FrameTime,
										   animTime);

		glm::vec3 finalPosition = glm::mix(m_translations[p0Index].Value,
										   m_translations[p1Index].Value,
										   scaleFactor);

		return glm::translate(glm::mat4(1.0f), finalPosition);
	}

	glm::mat4 Bone::interpolateRotation(float animTime)
	{
		if (1 == m_numRotations)
		{
			auto rotation = glm::normalize(m_rotations[0].Value);
			return glm::toMat4(rotation);
		}

		int p0Index = getRotationIndex(animTime);
		int p1Index = p0Index + 1;
		float scaleFactor = getScaleFactor(m_rotations[p0Index].FrameTime,
										   m_rotations[p1Index].FrameTime,
										   animTime);

		glm::quat finalRotation = glm::slerp(m_rotations[p0Index].Value,
											 m_rotations[p1Index].Value,
											 scaleFactor);

		finalRotation = glm::normalize(finalRotation);
		return glm::toMat4(finalRotation);
	}

	glm::mat4 Bone::interpolateScale(float animTime)
	{
		if (1 == m_numScalings)
			return glm::scale(glm::mat4(1.0f), m_scales[0].Value);

		int p0Index = getScaleIndex(animTime);
		int p1Index = p0Index + 1;
		float scaleFactor = getScaleFactor(m_scales[p0Index].FrameTime,
										   m_scales[p1Index].FrameTime,
										   animTime);

		glm::vec3 finalScale = glm::mix(m_scales[p0Index].Value,
										m_scales[p1Index].Value,
										scaleFactor);

		return glm::scale(glm::mat4(1.0f), finalScale);
	}

	int Bone::getTranslationIndex(float animTime)
	{
		for (int index = 0; index < m_numTranslations - 1; ++index)
		{
			if (animTime < m_translations[index + 1].FrameTime)
				return index;
		}
		return 0;
	}

	int Bone::getRotationIndex(float animTime)
	{
		for (int index = 0; index < m_numRotations - 1; ++index)
		{
			if (animTime < m_rotations[index + 1].FrameTime)
				return index;
		}
		return 0;
	}

	int Bone::getScaleIndex(float animTime)
	{
		for (int index = 0; index < m_numScalings - 1; ++index)
		{
			if (animTime < m_scales[index + 1].FrameTime)
				return index;
		}
		return 0;
	}
}