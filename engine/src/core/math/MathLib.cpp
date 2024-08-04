#include <math.h>

#include "MathLib.h"

#include "scene/Camera.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Math"

namespace pio
{
	namespace Math
	{
		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation, glm::vec3& scale)
		{
			glm::mat4 localMatrix(transform);

			if (glm::epsilonEqual(localMatrix[3][3], static_cast<float>(0), glm::epsilon<float>()))
				return false;

			// Next take care of translation (easy).
			translation = glm::vec3(localMatrix[3]);
			localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

			// Now get scale and shear.
			glm::vec3 row[3];
			for (size_t i = 0; i < 3; ++i)
				for (size_t j = 0; j < 3; ++j)
					row[i][j] = localMatrix[i][j];

			// Compute X scale factor and normalize first row.
			scale.x = glm::length(row[0]);
			row[0] = Scale(row[0], static_cast<float>(1));
			// Now, compute Y scale and normalize 2nd row.
			scale.y = glm::length(row[1]);
			row[1] = Scale(row[1], static_cast<float>(1));
			// Next, get Z scale and normalize 3rd row.
			scale.z = glm::length(row[2]);
			row[2] = Scale(row[2], static_cast<float>(1));

			// Rotation as quaternion
			int i, j, k = 0;
			float root, trace = row[0].x + row[1].y + row[2].z;
			if (trace > static_cast<float>(0))
			{
				root = sqrt(trace + static_cast<float>(1));
				rotation.w = static_cast<float>(0.5) * root;
				root = static_cast<float>(0.5) / root;
				rotation.x = root * (row[1].z - row[2].y);
				rotation.y = root * (row[2].x - row[0].z);
				rotation.z = root * (row[0].y - row[1].x);
			} // End if > 0
			else
			{
				static int next[3] = { 1, 2, 0 };
				i = 0;
				if (row[1].y > row[0].x) i = 1;
				if (row[2].z > row[i][i]) i = 2;
				j = next[i];
				k = next[j];

				root = glm::sqrt(row[i][i] - row[j][j] - row[k][k] + static_cast<float>(1.0));

				rotation[i] = static_cast<float>(0.5) * root;
				root = static_cast<float>(0.5) / root;
				rotation[j] = root * (row[i][j] + row[j][i]);
				rotation[k] = root * (row[i][k] + row[k][i]);
				rotation.w = root * (row[j][k] - row[k][j]);
			} // End if <= 0

			return true;
		}

		glm::vec3 Math::Reminder(const glm::vec3& input, float reminder)
		{
			if (reminder < 0.f) { reminder = -reminder; }

			glm::vec3 result = input;
			glm::vec3 sign = glm::sign(result);

			auto calcReminder = [](float input, float sign, float reminder)
				{
					float val = input * sign;// val is >= 0.f
					if (val > reminder)
						val = val - reminder * int(val / reminder);
					return val * sign;
				};

			result.x = calcReminder(result.x, sign.x, reminder);
			result.y = calcReminder(result.y, sign.y, reminder);
			result.z = calcReminder(result.z, sign.z, reminder);

			return result;
		}

		glm::mat4 PerspectiveMat(float fov, float aspect, float near, float far)
		{
			if (LessEqual(fov, 0.f) || LessEqual(aspect, 0.f))
			{
				LOGE("err! invalid input, fov[%f], aspect[%f]", fov, aspect);
				return glm::mat4(1.f);
			}

			if (LessEqual(near, 0.f) || LessEqual(far, 0.f))
			{
				LOGE("err! invalid input, near[%f] and far[%f]", near, far);
				return glm::mat4(1.f);
			}

			return glm::perspective(glm::radians(fov), aspect, near, far);
		}

		glm::mat4 OrthoMat(float left, float right, float bottom, float top, float near, float far)
		{
			return glm::ortho(left, right, bottom, top, near, far);
		}

		glm::mat4 ViewportMat(const Viewport& vp)
		{
			glm::vec4 col0 = glm::vec4(float(vp.w()) / 2.f, 0.f, 0.f, 0.f);
			glm::vec4 col1 = glm::vec4(0.f, float(vp.h()) / 2.f, 0.f, 0.f);
			glm::vec4 col2 = glm::vec4(0.f, 0.f, 0.5f, 0.f);
			glm::vec4 col3 = glm::vec4(float(vp.x()) + float(vp.w()) / 2.f,
				                       float(vp.y()) + float(vp.h()) / 2.f,
				                       0.5f, 1.f);
			return glm::mat4(col0, col1, col2, col3);
		}

		glm::ivec2 ScreenPtToViewportPt(const glm::vec2& screenPt, const LayoutParams& param)
		{
			return glm::ivec2(screenPt.x - param.Viewport.x(), param.Viewport.h() - (screenPt.y - param.Position.Top));
		}

		glm::vec2 ScreenPtToVertex(uint32_t x, uint32_t y, uint32_t screenWidth, uint32_t screenHeight)
		{
			float halfW = float(screenWidth) / 2.f;
			float halfH = float(screenHeight) / 2.f;
			return glm::vec2((float(x) - halfW) / halfW,
				             (halfH - float(y)) / halfH);
		}

		glm::vec2 WorldPosToScreenPt(const glm::vec3& worldPos, const glm::mat4& mvpMat, const glm::mat4& vpMat, const glm::uvec2& windowSize)
		{
			glm::vec4 clipPos = mvpMat * glm::vec4(worldPos, 1.f);
			glm::vec4 cvv = glm::vec4(clipPos.x / clipPos.w, clipPos.y / clipPos.w, clipPos.z / clipPos.w, 1.f);
			glm::vec4 screen = vpMat * cvv;
			// Window's origin is the left-top corner
			screen.y = windowSize.y - screen.y;
			return glm::vec2(screen.x, screen.y);
		}
	}
}