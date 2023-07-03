#ifndef __RENDER_LIGHT_H__
#define __RENDER_LIGHT_H__

#include <glm/glm.hpp>
#include <atomic>
#include <memory>

#include "LightDef.h"

namespace pio
{
	class  Shader;
	class  GfxFrameBuffer;
	struct RenderParam;

	enum LightDir : uint8_t
	{
		LIGHT_DIR_POSITIVE_X = 0, // GL_TEXTURE_CUBE_MAP_POSITIVE_X
		LIGHT_DIR_NEGATIVE_X, // GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		LIGHT_DIR_POSITIVE_Y, // GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		LIGHT_DIR_NEGATIVE_Y, // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		LIGHT_DIR_NEGATIVE_Z, // GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		LIGHT_DIR_POSITIVE_Z  // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	class Light
	{
	public:
		Light();
		virtual ~Light();

		static std::shared_ptr<Light> createLight(LightType type);

		virtual void dealShader(RenderParam &param, std::shared_ptr<Shader> &shader, uint32_t texUnit = 0) = 0;
		virtual void bind(RenderParam &param, std::shared_ptr<Shader> &shader, uint32_t slot) = 0;
		virtual void unbind(RenderParam &param) = 0;

		const glm::mat4 &getViewMat();
		const glm::mat4 &getPrjMat();
		void setPosition(const glm::vec3 &pos);
		void setDirection(const glm::vec3 &dir);
		void setNear(float near);
		void setFar(float far);

		void setColor(const glm::vec3 &ka, const glm::vec3 &kd, const glm::vec3 &ks);
		void setIntensity(float ia, float id, float is);

		void initDepthBuffer(uint32_t width, uint32_t height, const RenderParam &param);

		template<class T>
		bool is() const;

		template<class T>
		T *to() const;

		inline const std::shared_ptr<GfxFrameBuffer> &getDepthFbo() { return m_depthFbo; }

		inline void setShininess(float shininess) { m_shininess = shininess; }

		inline const glm::vec3 &position() { return m_position; }
		inline const glm::vec3 &direction() { return m_direction; }

		inline const glm::vec3 &ambient() { return m_ka; }
		inline const glm::vec3 &diffuse() { return m_kd; }
		inline const glm::vec3 &specular() { return m_ks; }

		inline float near() const { return m_near; }
		inline float far() const  { return m_far; }

		inline float ambientIntensity() { return m_ia; }
		inline float diffuseIntensity() { return m_id; }
		inline float specularIntensity() { return m_is; }

		inline float shininess() { return m_shininess; }

		inline LightType type() { return m_type; }

	protected:
		virtual void calcMatrix() = 0;

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

		float m_near, m_far;

		glm::mat4 m_viewMat, m_prjMat;
		std::atomic<bool> m_dataChange;

		LightType m_type;

		std::shared_ptr<GfxFrameBuffer> m_depthFbo;
	};

	template<class T>
	bool Light::is() const
	{
		return false;
	}

	template<class T>
	T *Light::to() const
	{
		return nullptr;
	}
}

#endif