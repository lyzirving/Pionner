#pragma once

#include "Component.h"

namespace pio
{
	class TransformComponent : public Component
	{
		RTTR_ENABLE(Component)
	public:
		TransformComponent() : Component() {};
		virtual ~TransformComponent() = default;
		virtual void OnAttach() override;
		virtual void OnTick() override;

		const glm::mat4& GetMat() const { return m_TransformMat; }
		bool AnyChange() const { return m_bDirty; }

		PIO_DEFINE_PROPERTY(glm::vec3, Position, glm::vec3(0.f))
		PIO_DEFINE_PROPERTY(glm::vec3, Scale, glm::vec3(1.f))
		PIO_DEFINE_PROPERTY(Rotator, Rotation)
		PIO_DEFINE_PROPERTY(Mobility, Mobility, Mobility_Static)

	private:		
		void Invalidate(bool val = true) { m_bDirty = val; }

	private:
		glm::mat4 m_TranslateMat{ 1.f };
		glm::mat4 m_ScaleMat{ 1.f };
		glm::mat4 m_TransformMat{ 1.f };
		bool m_bDirty{ true };
	};
}