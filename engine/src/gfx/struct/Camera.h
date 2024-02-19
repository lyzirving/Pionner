#ifndef __PIONNER_GFX_STRUCT_CAMERA_H__
#define __PIONNER_GFX_STRUCT_CAMERA_H__

#include "Frustum.h"

#include "gfx/renderer/RenderState.h"
#include "gfx/rhi/Uniform.h"
#include "core/math/SphereCoord.h"

namespace pio
{
	enum class CameraId : uint8_t { MAIN = 0, CNT };
	enum class ProjectionType { None = 0, Perspective, Orthographic };

	struct CameraUD
	{
		PIO_UNIFORM_STRUCT_DECLARE(CameraUD)

		glm::mat4 ViewMat{ 1.f };
		glm::mat4 PrjMat{ 1.f };
		glm::mat4 OrthoMat{ 1.f };
		glm::vec3 CameraPosition{ 0.f };
		float FrustumFar{ 0.f };
	};

	class Camera
	{
	private:
		// ---------------------- class CameraImpl -------------------------------
		// -----------------------------------------------------------------------
		class CameraImpl
		{
			friend class Camera;
		public:
			struct CamState
			{
				glm::vec3 m_position{ 0.f };
				glm::vec3 m_lookAt{ 0.f };
			};

			CameraImpl();
			CameraImpl(const CameraImpl &rhs);
			CameraImpl &operator=(const CameraImpl &rhs);
			~CameraImpl();

		private:
			CamState acquireState();
			void applyState(const CamState &state);
			void addPosDiff(float thetaDiff, float phiDiff, float rDiff);
			void addRadiusDiff(float delta);
			void setPosition(float theta, float phi, float r);
			void setPosition(const SphereCoord &position);
			void setPosition(const glm::vec3 &pos);
			void setLookAt(const glm::vec3 &lookAt);
			void calcViewMat();

			const glm::mat4 &getViewMat() const { return m_viewMat; }
			const glm::vec3 &getCamPos() const { return m_camPos; }
			const SphereCoord &getCamPosSC() const { return m_camPosSpherical; }

		private:
			static glm::mat4 CalcViewMat(const SphereCoord &position, const glm::vec3 &lookAt);
			static glm::mat4 CalcViewMat(const glm::vec3 &position, const glm::vec3 &lookAt);

		private:
			glm::vec3 m_camPos{ 0.f };
			SphereCoord m_camPosSpherical{};
			// m_lookAt is set (0.f, 0.f, 0.f) by default
			// -(m_lookAt - m_camPos) is camera's +z
			glm::vec3 m_lookAt{ 0.f };

			// camera's +x
			glm::vec3 m_camRight{ 0.f };
			// camera's +y
			glm::vec3 m_camUp{ 0.f };
			glm::mat4 m_viewMat{ 1.f };
		};
		// -----------------------------------------------------------------------
		// -----------------------------------------------------------------------

	public:
		Camera();
		Camera(CameraId id);
		Camera(const Camera &rhs);
		Camera &operator=(const Camera &rhs);
		virtual ~Camera();

		/*
	     * @brief: add difference to camera's position, delta is screen coordinate.
	     * @param: deltaX, scroll delta distance on horizontal, +x is go right on screen.
	     *         deltaY, scroll delta distance on vertical, +y is go down on screen.
	     */
		void addPosDiff(float deltaX, float deltaY);
		void addRadiusDiff(float delta);
		void flush();

		/*
		* @brief: calculate view matrix by specific input
		*/ 
		static glm::mat4 GetViewMat(const SphereCoord &position, const glm::vec3 &lookAt = glm::vec3(0.f));
		static glm::mat4 GetViewMat(const glm::vec3 &position, const glm::vec3 &lookAt = glm::vec3(0.f));
		static glm::mat4 GetOrtho(float l, float r, float b, float t);

	public:
		inline void setVisible(bool b) { m_visible = b; }
		inline void setPosition(float theta, float phi, float r) { m_camImpl.setPosition(theta, phi, r); }
		inline void setPosition(const SphereCoord &position) { m_camImpl.setPosition(position); }
		inline void setPosition(const glm::vec3 &pos) { m_camImpl.setPosition(pos); }
		inline void setLookAt(const glm::vec3 &lookAt) { m_camImpl.setLookAt(lookAt); }

		inline void setFov(float fov) { m_frustum.setFov(fov); }
		inline void setAspect(float aspect) { m_frustum.setAspect(aspect); }
		inline void setNearFar(float near, float far) { m_frustum.setNearFar(near, far); }
		inline void setOrtho(float l, float r, float b, float t) { m_frustum.setOrtho(l, r, b, t); }
		inline void setViewport(int32_t x, int32_t y, int32_t w, int32_t h) { m_viewport = Viewport{ x, y, w, h }; }

		inline CameraId getCameraId() const { return m_id; }
		inline bool isVisible() const { return m_visible; }

		inline float near() const { return m_frustum.near(); }
		inline float far() const { return m_frustum.far(); }
		inline float fov() const { return m_frustum.fov(); }

		inline float top() const { return m_frustum.top(); }
		inline float bottom() const { return m_frustum.bottom(); }
		inline float right() const { return m_frustum.right(); }
		inline float left() const { return m_frustum.left(); }

		inline const glm::vec3 &getCameraPos() const { return m_camImpl.getCamPos(); }
		inline const SphereCoord &getCameraPosSC() const { return m_camImpl.getCamPosSC(); }

		inline const glm::mat4 &getViewMat() const { return m_camImpl.getViewMat(); }
		inline const glm::mat4 &getPrjMat() const { return m_frustum.getPerspectMat(); }
		inline const glm::mat4 &getOrthoMat() const { return m_frustum.getOrthoMat(); }
		inline const Viewport &getViewport() const { return m_viewport; }

	protected:
		bool m_visible{ true };
		CameraId m_id{ CameraId::MAIN };
		CameraImpl m_camImpl{};
		Frustum m_frustum{};
		Viewport m_viewport{};
	};
}

#endif