#include "UiDef.h"

namespace pio
{
	glm::ivec2 ScreenToViewport(const glm::vec2 &screenPt, const WindowLayoutParams &param)
	{
		return glm::ivec2(screenPt.x - param.Viewport.X, param.Viewport.Height - (screenPt.y - param.Position.Top));
	}

	glm::vec2 ScreenToVertex(uint32_t x, uint32_t y, uint32_t screenWidth, uint32_t screenHeight)
	{
		glm::vec2 ret;
		float halfWidth = float(screenWidth) / 2.f;
		float halfHeight = float(screenHeight) / 2.f;
		ret.x = (float(x) - halfWidth) / halfWidth;
		ret.y = (halfHeight - float(y)) / halfHeight;
		return ret;
	}

	glm::vec2 WorldToScreenPos(const glm::vec3 &worldPos, const glm::mat4 &mvpMat, const glm::mat4 &vpMat, const glm::uvec2 &windowSize)
	{
		glm::vec4 clipPos = mvpMat * glm::vec4(worldPos, 1.f);
		glm::vec4 cvv = glm::vec4(clipPos.x / clipPos.w, clipPos.y / clipPos.w, clipPos.z / clipPos.w, 1.f);
		glm::vec4 screen = vpMat * cvv;
		// Window's origin is the left-top corner
		screen.y = windowSize.y - screen.y;
		return glm::vec2(screen.x, screen.y);
	}
}


