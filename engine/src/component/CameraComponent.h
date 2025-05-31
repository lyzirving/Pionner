#pragma once

#include "Component.h"

namespace pio
{
	class UniformBlock;
	class UniformBuffer;

	class CameraComponent : public Component
	{
		RTTR_ENABLE(Component)
	public:
		CameraComponent();
		virtual ~CameraComponent() = default;
		virtual void OnAttach() override;
		virtual void OnTick();

		float FrustTop() const { return (m_PrjType == ProjectionType_Perspective) ? m_Near * std::tan(glm::radians(m_Fov * 0.5f)) : m_Size; }
		float FrustBottom() const { return -FrustTop(); }
		float FrustRight() const { return FrustTop() * m_Aspect; }
		float FrustLeft() const { return -FrustRight(); }

		const glm::mat4& GetViewMat() const { return m_ViewMat; }
		const glm::mat4& GetPersMat() const { return m_PersMat; }
		const glm::mat4& GetOrthoMat() const { return m_OrthoMat; }
		const Ref<UniformBuffer>& GetUBuffer() const { return m_UBuffer; }

		const glm::vec3& GetCameraRight() const { return m_CamRight; }
		const glm::vec3& GetCameraUp() const { return m_CamUp; }
		const glm::vec3& GetCameraFront() const { return m_CamFront; }
		const glm::vec3& GetViewDir() const { return m_ViewDir; }

	private:
		enum CameraAttrBits : uint8_t
		{
			CameraAttrBits_Pose = 0,
			CameraAttrBits_Frustum,
			CameraAttrBits_Pers,
			CameraAttrBits_Ortho,
			CameraAttrBits_Num
		};
		using CameraAttrs = std::bitset<CameraAttrBits_Num>;

	private:
		void CreateUBuffer();
		void CalcCameraPose();
		void CalcPersFrustum();
		void CalcOrthoFrustum();
		void Flush();

		PIO_DEFINE_PROPERTY(glm::vec3, Position, glm::vec3(0.f))
		PIO_DEFINE_PROPERTY(Rotator, Rotation)
		PIO_DEFINE_PROPERTY(ProjectionType, PrjType, ProjectionType_Perspective)
		// property for frustum		
		PIO_DEFINE_PROPERTY(float, Near, 0.1f)
		PIO_DEFINE_PROPERTY(float, Far, 100.f)
		PIO_DEFINE_PROPERTY(float, Aspect, 1.f)
		// property for perspective frustum		
		PIO_DEFINE_PROPERTY(float, Fov, 60.f)
		// property for orthogonal frustum
		PIO_DEFINE_PROPERTY(float, Size, 1.f)

		glm::vec3 m_CamRight{ World::Right };
		glm::vec3 m_CamUp{ World::Up };
		glm::vec3 m_CamFront{ World::Forward };
		glm::vec3 m_ViewDir{ -World::Forward };
		glm::mat4 m_ViewMat{ 1.f };
		glm::mat4 m_PersMat{ 1.f }, m_OrthoMat{ 1.f };

		CameraAttrs m_AttrsChange;

		Ref<UniformBlock> m_UBlock;
		Ref<UniformBuffer> m_UBuffer;
	};
}