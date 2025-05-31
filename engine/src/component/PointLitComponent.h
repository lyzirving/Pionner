#pragma once

#include "Component.h"
#include "gfx/GfxDef.h"

namespace pio
{
	class PointLitComponent : public Component
	{
		RTTR_ENABLE(Component)
	public:
		PointLitComponent() : Component() {}
		virtual ~PointLitComponent() = default;

		void ApplyAttrsChange();

		PIO_DEFINE_PROPERTY(glm::vec3, Position, glm::vec3(0.f))
		PIO_DEFINE_PROPERTY(glm::vec3, Color, glm::vec3(1.f))
		PIO_DEFINE_PROPERTY(int32_t, Index, 0)
		PIO_DEFINE_PROPERTY(float, Intensity, 1.f)
		PIO_DEFINE_PROPERTY(float, Radius, 2.f)
		PIO_DEFINE_PROPERTY(float, Falloff, 1.f)
		PIO_DEFINE_PROPERTY(float, Bias, 0.09f)
		PIO_DEFINE_PROPERTY(float, ShadowIntensity, 1.f)
		PIO_DEFINE_PROPERTY(ShadowMode, Mode, ShadowMode_Hard)
		PIO_DEFINE_PROPERTY(bool, bCastShadow, true)
	};
}