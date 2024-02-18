#include "UiDef.h"

namespace pio
{
	namespace UiDef
	{
		glm::uvec2 ScreenToViewport(const glm::vec2 &screenPt, const LayoutParams &param)
		{
			return glm::uvec2(screenPt.x - param.Viewport.X, param.Viewport.Height - (screenPt.y - param.Position.Top));
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
	}
}


