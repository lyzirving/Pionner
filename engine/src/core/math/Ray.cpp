#include "Ray.h"

#include "gfx/struct/Camera.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Ray"

namespace pio
{
	Ray Ray::BuildFromScreen(const glm::vec2 &viewportPt, const Camera &camera)
	{
		float far = camera.far();
		float near = camera.near();
		float left = camera.left();
		float right = camera.right();
		float top = camera.top();
		float bottom = camera.bottom();
		// camera position in world space
		glm::vec3 camPos = camera.getCameraPos();
		const Viewport &vp = camera.getViewport();
		const glm::mat4 &viewMat = camera.getViewMat();

		// origin of OpenGL's viewport is left-bottom.
		glm::vec2 pt = viewportPt;
		// transform pt from screen to CVV
		pt.x = (pt.x / (float)vp.Width) * 2.f - 1.f;
		pt.y = (pt.y / (float)vp.Height) * 2.f - 1.f;

		// transform pt to project plane
		pt.x = (pt.x + 1.f) * 0.5f * (right - left) + left;
		pt.y = (pt.y + 1.f) * 0.5f * (top - bottom) + bottom;

		// pt in camera space on near plane
		glm::vec3 camPt = glm::vec3(pt.x, pt.y, -near);

		// --------------transform pt in camera space into world space-------------
		// get rotation matrix
		glm::mat3 rotMat = glm::mat3(viewMat[0].x, viewMat[0].y, viewMat[0].z,
									 viewMat[1].x, viewMat[1].y, viewMat[1].z,
									 viewMat[2].x, viewMat[2].y, viewMat[2].z);
		glm::mat3 invRot = glm::inverse(rotMat);
		camPt = invRot * camPt;
		camPt += camPos;

		// set the pt in frustum for GDebuger visualization
		camPt.z -= near;
		camPos.z -= near;

		return Ray(camPos, glm::normalize(camPt - camPos));
	}
}