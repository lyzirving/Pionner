#ifndef __PIONNER_GFX_DATA_CAMERA_DATA_H__
#define __PIONNER_GFX_DATA_CAMERA_DATA_H__

#include "DataWrap.h"

namespace pio
{
	struct CameraUD
	{
		PIO_UNIFORM_STRUCT_DECLARE(CameraUD)

		glm::mat4 ViewMat{ 1.f };
		glm::mat4 PrjMat{ 1.f };
		glm::mat4 OrthoMat{ 1.f };
		glm::vec3 CameraPosition{ 0.f };
		float FrustumFar{ 0.f };
		ProjectionType PrjType{ ProjectionType_Perspective };
	};

	class CameraData : public DataWrap
	{
	public:
		CameraData();
		~CameraData() = default;

		virtual void setUp(Ref<RenderContext> &context) override;
		virtual Ref<UniformBuffer> &uniformBuffer() override { return m_uniformBuffer; }

	private:
		CameraUD m_uniformData;
		Ref<UniformBuffer> m_uniformBuffer;

	private:
		friend class Camera;
	};
}

#endif