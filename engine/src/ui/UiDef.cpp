#include "UiDef.h"
#include "UiDef.h"

namespace pio
{
	namespace UiDef
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

		glm::vec2 UiDef::MoveToOrigin(const glm::vec2 &pt, const glm::vec2 &orign)
		{
			return glm::vec2(pt.x - orign.x, pt.y - orign.y);
		}
	}
}


