#pragma once

#include "Component.h"
#include "gfx/GfxDef.h"

namespace pio
{
	class IndirectLight;
	class IndirectLightComponent : public Component
	{
		RTTR_ENABLE(Component)
	public:
		IndirectLightComponent() : Component() {}
		virtual ~IndirectLightComponent() = default;
		virtual void OnAttach() override;
		virtual void OnTick() override;

		const Ref<IndirectLight>& GetIndirectLight() const { return m_IndirectLight; }

	private:
		Ref<IndirectLight> m_IndirectLight;
	};
}