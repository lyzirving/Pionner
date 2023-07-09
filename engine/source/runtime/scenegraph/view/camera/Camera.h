#ifndef __PIONNER_SCENEGRAPH_VIEW_CAMERA_H__
#define __PIONNER_SCENEGRAPH_VIEW_CAMERA_H__

#include "CameraImpl.h"
#include "Frustum.h"

namespace pio
{
	namespace sgf
	{
		class Camera
		{
		public:
			Camera();
			~Camera();

			void setPosition(float theta, float phi, float r);
			void setPosition(const glm::vec3 &pos);
			void setLookAt(const glm::vec3 &lookAt);

			void setFov(float fov);
			void setAspect(float aspect);
			void setNearFar(float near, float far);
			void setOrtho(float l, float r, float b, float t);

			const glm::mat4 &getViewMat();
			const glm::mat4 &getPrjMat();
			const glm::mat4 &getOrthoMat();
			const glm::vec3 &getCamPos();

		private:
			CameraImpl m_camImpl{};
			Frustum    m_frustum{};
		};
	}
}

#endif