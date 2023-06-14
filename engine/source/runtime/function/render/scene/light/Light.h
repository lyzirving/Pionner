#ifndef __RENDER_LIGHT_H__
#define __RENDER_LIGHT_H__

#include <glm/glm.hpp>
#include <memory>

#include "LightDef.h"

namespace Pionner
{
	class Shader;

	class Light
	{
	public:
		Light();
		virtual ~Light();

		static std::shared_ptr<Light> createLight(LightType type);

		inline void setPosition(const glm::vec3 &pos) { m_position = pos; }
		inline void setDirection(const glm::vec3 &dir) { m_direction = dir; }
		inline void setShininess(float shininess) { m_shininess = shininess; }

		inline const glm::vec3 &position() { return m_position; }
		inline const glm::vec3 &direction() { return m_direction; }

		inline const glm::vec3 &ambient() { return m_ka; }
		inline const glm::vec3 &diffuse() { return m_kd; }
		inline const glm::vec3 &specular() { return m_ks; }

		inline float ambientIntensity() { return m_ia; }
		inline float diffuseIntensity() { return m_id; }
		inline float specularIntensity() { return m_is; }

		inline float shininess() { return m_shininess; }

		inline LightType type() { return m_type; }

		virtual void dealShader(const std::shared_ptr<Shader> &shader) = 0;

		void setColor(const glm::vec3 &ka, const glm::vec3 &kd, const glm::vec3 &ks);
		void setIntensity(float ia, float id, float is);

		template<class T>
		bool is() const;

	protected:
		glm::vec3 m_position;

		glm::vec3 m_direction;
		/**
		 * @brief: m_ka, m_kd and m_ks are the color of ambient, diffuse and specular.
		 *         They are RGB format, and range in [0, 1].
		 */
		glm::vec3 m_ka, m_kd, m_ks;

		/**
		 * @brief: m_ia, m_id and m_is are the intensity of ambient, diffuse and specular.
		 *         Their value should be in the range of [0, 1].
		 */
		float m_ia, m_id, m_is;

		/**
		 * @brief: m_shininess is used to control the radius of specular.
		 *         It is used as an exponent to calculate the power of dot(bisector, normal).
		 */
		float m_shininess;

		LightType m_type;
	};

	template<class T>
	bool Light::is() const
	{
		return false;
	}
}

#endif