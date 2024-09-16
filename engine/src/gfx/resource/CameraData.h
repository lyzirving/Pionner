#ifndef __PIONNER_GFX_RESOURCE_CAMERA_DATA_H__
#define __PIONNER_GFX_RESOURCE_CAMERA_DATA_H__

#include "gfx/rhi/UniformBuffer.h"

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
}

#endif