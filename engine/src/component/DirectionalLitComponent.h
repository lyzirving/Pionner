#pragma once

#include "Component.h"
#include "gfx/GfxDef.h"

namespace pio
{
	class UniformBlock;
	class UniformBuffer;
	class ShadowMap;
	class TransformComponent;

	class DirectionalLitComponent : public Component
	{
		RTTR_ENABLE(Component)
	public:
		DirectionalLitComponent() : Component() {}
		virtual ~DirectionalLitComponent() = default;
		virtual void OnAttach() override;
		virtual void OnTick() override;

		void OnTransformChange(const Ref<TransformComponent>& comp);

		const Ref<UniformBuffer>& GetUBuffer() const { return m_UBuffer; }
		const Ref<ShadowMap>& GetShadowMap() const { return m_ShadowMap; }

	private:
		void CreateUBuffer();
		void CreateShadowMap();

		PIO_DEFINE_PROPERTY(glm::vec3, Direction, World::Down)
		PIO_DEFINE_PROPERTY(glm::vec3, Color, glm::vec3(0.6f))
		PIO_DEFINE_PROPERTY(float, Intensity, 1.f)
		PIO_DEFINE_PROPERTY(float, Bias, 0.f)
		PIO_DEFINE_PROPERTY(float, NormalBias, 0.f)
		PIO_DEFINE_PROPERTY(ShadowMode, Mode, ShadowMode_Hard)
		PIO_DEFINE_PROPERTY(float, ShadowIntensity, 1.f)
		PIO_DEFINE_PROPERTY(bool, bCastShadow, true)

	private:
		Ref<UniformBlock> m_UBlock;
		Ref<UniformBuffer> m_UBuffer;
		Ref<ShadowMap> m_ShadowMap;
	};
}