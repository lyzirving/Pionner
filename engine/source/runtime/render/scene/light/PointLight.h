#ifndef __RENDER_POINT_LIGHT_H__
#define __RENDER_POINT_LIGHT_H__

#include "Light.h"

namespace pio
{
	class PointLight : public Light
	{
	public:
		PointLight();
		PointLight(const glm::vec3 &position);
		~PointLight();

		virtual void dealShader(RenderParam &param, std::shared_ptr<Shader> &shader, uint32_t texUnit = 0) override;
		virtual void bind(RenderParam &param, std::shared_ptr<Shader> &shader, uint32_t slot) override;
		virtual void unbind(RenderParam &param) override;

		void setAttenuation(float constVal, float linearVal, float quadVal);

		const glm::mat4 &getLightMat(LightDir dir);

	protected:
		virtual void calcMatrix() override;

	protected:
		float m_attenParamConst, m_attenParamLinear, m_attenParamQuad;

	private:
		glm::mat4 m_lightMat[6];
	};

	template<>
	bool Light::is<PointLight>() const;

	template<>
	PointLight *Light::to<PointLight>() const;
}

#endif