#pragma once

#include "Component.h"

namespace pio
{
	class UniformBlock;
	class UniformBuffer;
	class PointLitShadowMap;

	class PointLitGroupComponent : public Component
	{
		RTTR_ENABLE(Component)
	public:
		PointLitGroupComponent() : Component() {}
		virtual ~PointLitGroupComponent() = default;
		virtual void OnAttach() override;
		virtual void OnTick() override;

		Ref<UniformBlock> GetUBlock() { return m_UBlock; }
		const Ref<UniformBlock>& GetUBlock() const { return m_UBlock; }

		Ref<UniformBuffer> GetUBuffer() { return m_UBuffer; }
		const Ref<UniformBuffer>& GetUBuffer() const { return m_UBuffer; }

		Ref<PointLitShadowMap> GetShadowMap() { return m_ShadowMap; }
		const Ref<PointLitShadowMap>& GetShadowMap() const { return m_ShadowMap; }

	private:
		void CreateBuffer();
		void CreateShadowMap();

	private:
		Ref<UniformBlock> m_UBlock;
		Ref<UniformBuffer> m_UBuffer;
		Ref<PointLitShadowMap> m_ShadowMap;

		PIO_DEFINE_PROPERTY(int32_t, Num, 0)
		PIO_DEFINE_PROPERTY(float, Fov, 90.f)
		PIO_DEFINE_PROPERTY(float, Near, 0.1f)
		PIO_DEFINE_PROPERTY(float, Far, 100.f)
	};
}