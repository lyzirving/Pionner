#ifndef __RENDER_POINT_LIGHT_H__
#define __RENDER_POINT_LIGHT_H__

#include "Light.h"

namespace Pionner
{
	class PointLight : public Light
	{
	public:
		PointLight();
		PointLight(const glm::vec3 &position);
		~PointLight();

		virtual void dealShader(const std::shared_ptr<Shader> &shader) override;

		void setAttenuation(float constVal, float linearVal, float quadVal);

	protected:
		float m_attenParamConst, m_attenParamLinear, m_attenParamQuad;
	};

	template<>
	bool Light::is<PointLight>() const;
}

#endif