#ifndef __PIONNER_GFX_DATA_CAMERA_DATA_H__
#define __PIONNER_GFX_DATA_CAMERA_DATA_H__

#include "CRenderData.h"

namespace pio
{
	class Camera;

	struct CCameraUD
	{
		PIO_UNIFORM_STRUCT_DECLARE(CCameraUD)

		glm::mat4 ViewMat{ 1.f };
		glm::mat4 PrjMat{ 1.f };
		glm::mat4 OrthoMat{ 1.f };
		glm::vec3 CameraPosition{ 0.f };
		float FrustumFar{ 0.f };
		ProjectionType PrjType{ ProjectionType_Perspective };
	};

	class CameraData : public CRenderData
	{	
	public:
		CameraData();
		CameraData(const std::string &name);
		~CameraData();

		void set(const Camera &camera);

	protected:
		virtual void initData() override;

	private:
		CCameraUD m_uniformData;

	private:
		static uint32_t k_CamDataNum;
	};
}

#endif