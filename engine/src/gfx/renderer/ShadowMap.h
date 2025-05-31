#pragma once

#include "FrameBufferProxy.h"

namespace pio
{
	class UniformBlock;
	class UniformBuffer;

	class ShadowMap : public FrameBufferProxy
	{
		RTTR_ENABLE(FrameBufferProxy)
	public:
		ShadowMap(const Ref<RenderContext>& context, const std::string& name);
		virtual ~ShadowMap() = default;

		void OnTick();

		Ref<UniformBuffer> GetUBuffer() const { return m_UBuffer; }

	private:
		void CreateFrameBuff();
		void CreateUBuffer();
		void UpdateVirtualCamPose();
		void UpdateFrustum();

		PIO_DEFINE_PROPERTY(glm::vec3, VirtualPosition, glm::vec3(0.f))
		PIO_DEFINE_PROPERTY(glm::vec3, ViewDirection, -World::Up)
		PIO_DEFINE_PROPERTY(glm::vec2, MapSize, glm::vec2(0.f))
		PIO_DEFINE_PROPERTY(AABB, SceneBoundingBox)

	protected:
		Ref<UniformBlock> m_UBlock;
		Ref<UniformBuffer> m_UBuffer;

		glm::mat4 m_ViewMat{ 1.f };
		glm::mat4 m_PrjMat{ 1.f };
		glm::vec3 m_VirtualCamRight{ World::Right };
		glm::vec3 m_VirtualCamUp{ World::Up };
		glm::vec3 m_VirtualCamFront{ World::Forward };
		bool m_bVirtualCamPoseChange{ true };
		bool m_bSceneBoundingChange{ true };
	};
}