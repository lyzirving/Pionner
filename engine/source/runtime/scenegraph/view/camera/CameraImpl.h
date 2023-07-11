#ifndef __PIONNER_SCENEGRAPH_VIEW_CAMERA_IMPL_H__
#define __PIONNER_SCENEGRAPH_VIEW_CAMERA_IMPL_H__

#include <bitset>

#include "scenegraph/SceneGrfDef.h"
#include "core/struct/SphereCoord.h"

namespace pio
{
	namespace sgf
	{
		enum CamDataBit : uint8_t
		{
			CAM_BIT_POS,
			CAM_BIT_VIEW_MAT,
			CAM_DATA_BITS
		};

		class CameraImpl
		{
			friend class Camera;
		public:
			void setPosition(float theta, float phi, float r);
			void setPosition(const glm::vec3 &pos);
			void setLookAt(const glm::vec3 &lookAt);

			const glm::mat4 &getViewMat();
			const glm::vec3 &getCamPos();

		private:
			CameraImpl();
			~CameraImpl();

		private:
			void calcViewMat();

		private:
			glm::vec3   m_camPos{ 0.f, 0.f, 0.f };
			SphereCoord m_camPosSpherical{ 50.f, 320.f, 7.f };
			// m_lookAt is set (0.f, 0.f, 0.f) by default
			// -(m_lookAt - m_camPos) is camera's +z
			glm::vec3   m_lookAt{ 0.f };

			// camera's +x
			glm::vec3 m_camRight{ 0.f };
			// camera's +y
			glm::vec3 m_camUp{ 0.f };

			glm::mat4 m_viewMat{ 1.f };

			std::bitset<CAM_DATA_BITS> m_dirty{};
		};
	}
}

#endif