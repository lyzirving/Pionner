#ifndef __RENDER_DIRECTION_LIGHT_H__
#define __RENDER_DIRECTION_LIGHT_H__

#include "Light.h"

namespace Pionner
{
	class DirectionLight : public Light
	{
	public:
		DirectionLight();
		DirectionLight(const glm::vec3 &direction);
		~DirectionLight();

		virtual void dealShader(const std::shared_ptr<Shader> &shader) override;

	protected:
		virtual void calcMatrix() override;
	};

	template<>
	bool Light::is<DirectionLight>() const;

	template<>
	DirectionLight *Light::to<DirectionLight>() const;
}

#endif