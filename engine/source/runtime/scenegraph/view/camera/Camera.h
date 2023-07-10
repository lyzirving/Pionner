#ifndef __PIONNER_SCENEGRAPH_VIEW_CAMERA_H__
#define __PIONNER_SCENEGRAPH_VIEW_CAMERA_H__

#include <memory>
#include <vector>

#include "CameraImpl.h"
#include "Frustum.h"

namespace pio
{
	namespace render
	{
		class Render;
	}

	namespace sgf
	{
		class Layer;

		class Camera
		{
		public:
			Camera(CameraId id);
			virtual ~Camera();

			virtual void update(const std::vector<std::shared_ptr<Layer>> &layers, RenderInfo &info) = 0;
			virtual void release() = 0;

			void setPosition(float theta, float phi, float r);
			void setPosition(const glm::vec3 &pos);
			void setLookAt(const glm::vec3 &lookAt);

			void setFov(float fov);
			void setAspect(float aspect);
			void setNearFar(float near, float far);
			void setOrtho(float l, float r, float b, float t);

			inline CameraId getCameraId() { return m_id; }
			inline bool     isVisible() { return m_visible; }
			inline void     setVisible(bool b) { m_visible = b; }
			inline void     setViewport(int x, int y, int width, int height)
			{
				m_viewport.x = x; m_viewport.y = y;
				m_viewport.width = width; m_viewport.height = height;
			}
			
			const glm::mat4 &getViewMat();
			const glm::mat4 &getPrjMat();
			const glm::mat4 &getOrthoMat();
			const glm::vec3 &getCamPos();

		protected:
			bool       m_visible{ false };
			glm::vec4  m_clearColor{0.24f, 0.24f, 0.24f, 1.f};
			Viewport   m_viewport{ 0, 0, 0, 0 };
			CameraId   m_id{ CAM_ID_CNT };
			CameraImpl m_camImpl{};
			Frustum    m_frustum{};
			std::shared_ptr<render::Render> m_render{ nullptr };
		};
	}
}

#endif