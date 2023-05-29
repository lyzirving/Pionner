#ifndef __RENDER_POINT_LIGHT_H__
#define __RENDER_POINT_LIGHT_H__

#include "function/framework/world/light/Light.h"

namespace Pionner
{
	class PointLight : public Light
	{
	public:
		PointLight();
		PointLight(const glm::vec3 &position);
		~PointLight();

		inline void setPosition(const glm::vec3 &pos) { m_position = pos; }
		inline const glm::vec3 &position() { return m_position; }

		virtual void dealShader(const std::shared_ptr<Shader> &shader) override;

		void setAttenuation(float constVal, float linearVal, float quadVal);

	protected:
		glm::vec3 m_position;
		float m_attenParamConst, m_attenParamLinear, m_attenParamQuad;
	};

	template<>
	bool Light::is<PointLight>() const;
}

#endif