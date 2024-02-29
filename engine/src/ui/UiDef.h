#ifndef __PIONNER_UI_UIDEF_H__
#define __PIONNER_UI_UIDEF_H__

#include "asset/Asset.h"

namespace pio
{

	#define CLICK_INTERVAL (300)

	constexpr static const char *STR_AXIS_X = "AxisX";
	constexpr static const char *STR_AXIS_Y = "AxisY";
	constexpr static const char *STR_AXIS_Z = "AxisZ";

	constexpr static const char *STR_TORUS_X = "TorusX";
	constexpr static const char *STR_TORUS_Y = "TorusY";
	constexpr static const char *STR_TORUS_Z = "TorusZ";

	enum class CoordinateShape : uint8_t
	{
		Cylinder, Arrow, Num
	};

	struct LayoutPercentage
	{
		float Left{ 0.f };  // 0.f - 1.f
		float Top{ 0.f };   // 0.f - 1.f
		float Right{ 1.f }; // 0.f - 1.f
		float Bottom{ 1.f };// 0.f - 1.f

		LayoutPercentage() {}
		LayoutPercentage(float l, float t, float r, float b)
			: Left(Math::Clamp(l, 0.f, 1.f)), Top(Math::Clamp(t, 0.f, 1.f))
			, Right(Math::Clamp(r, 0.f, 1.f)), Bottom(Math::Clamp(b, 0.f, 1.f))
		{
		}
	};

	/*
	* @brief: Layout of a panel in window coordinate whose origin is left-top
	*/
	struct LayoutRect
	{
		uint32_t Left{ 0 };
		uint32_t Top{ 0 };
		uint32_t Right{ 0 };
		uint32_t Bottom{ 0 };

		LayoutRect() {}
		LayoutRect(uint32_t l, uint32_t t, uint32_t r, uint32_t b)
			: Left(l), Top(t), Right(r), Bottom(b) {}
		LayoutRect(const glm::ivec2 lt, uint32_t width, uint32_t height) 
			: Left(lt.x), Top(lt.y), Right(lt.x + width), Bottom(lt.y + height) {}

		uint32_t width() const { return Right - Left; }
		uint32_t height() const { return Bottom - Top; }
		bool contain(uint32_t x, uint32_t y) const { return x >= Left && x <= Right && y >= Top && y <= Bottom; }

		bool operator==(const LayoutRect &rhs) 
		{
			if (this == &rhs) return true;
			return this->Left == rhs.Left && this->Top == rhs.Top && this->Right == rhs.Right && this->Bottom == rhs.Bottom;
		}

		bool operator!=(const LayoutRect &rhs) { return !((*this) == rhs); }

		bool bIntersect(const LayoutRect &rhs)
		{
			glm::uvec2 lt = glm::uvec2(std::max(Left, rhs.Left), std::max(Top, rhs.Top));
			glm::uvec2 rb = glm::uvec2(std::min(Right, rhs.Right), std::min(Bottom, rhs.Bottom));
			return lt.x <= rb.x && lt.y <= rb.y;
		}

		void doUnion(const LayoutRect &rhs)
		{
			if (this != &rhs && bIntersect(rhs))
			{
				Left = std::min(Left, rhs.Left);
				Top = std::min(Top, rhs.Top);
				Right = std::max(Right, rhs.Right);
				Bottom = std::max(Bottom, rhs.Bottom);
			}
		}
	};

	/*
	* @brief: Layout of a viewport in coordinate whose origin is left-bottom
	*/
	struct LayoutViewport
	{
		uint32_t X{ 0 };
		uint32_t Y{ 0 };
		uint32_t Width{ 0 };
		uint32_t Height{ 0 };

		LayoutViewport() {}
		LayoutViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
			: X(x), Y(y), Width(width), Height(height) {}

		bool operator==(const LayoutViewport &rhs)
		{
			if (this == &rhs) return true;
			return this->X == rhs.X && this->Y == rhs.Y && this->Width == rhs.Width && this->Height == rhs.Height;
		}

		bool operator!=(const LayoutViewport &rhs) { return !((*this) == rhs); }
	};

	/*
	* @brief: WindowLayoutParams is mainly used for layout on the entire window.
	*		  WindowLayoutParams.Percentage defines the rect of panel on window by ratio in [0, 1].
	*		  WindowLayoutParams.Position is the pixel result of Percentage.
	*		  WindowLayoutParams.Viewport is the rendering viewport setup by this panel, whose origin is the left-bottom of panel.
	*/
	struct WindowLayoutParams
	{
		LayoutPercentage Percentage;//Layout ratio on entire window
		LayoutRect Position;//Position on entire window
		LayoutViewport Viewport;// Rendering panel on window

		WindowLayoutParams() : Percentage(), Position(), Viewport()
		{
		}

		WindowLayoutParams(float l, float t, float r, float b) 
			: Percentage(l, t, r, b), Position(), Viewport()
		{
		}

		void calculate(uint32_t winWidth, uint32_t winHeight)
		{
			Position.Left = winWidth * Percentage.Left;
			Position.Top = winHeight * Percentage.Top;
			Position.Right = winWidth * Percentage.Right;
			Position.Bottom = winHeight * Percentage.Bottom;

			Viewport.Width = Position.Right - Position.Left;
			Viewport.Height = Position.Bottom - Position.Top;
			Viewport.X = Position.Left;
			Viewport.Y = winHeight - Position.Top - Viewport.Height;
		}
	};

	namespace UiDef
	{
		glm::ivec2 ScreenToViewport(const glm::vec2 &screenPt, const WindowLayoutParams& param);
		// TODO: make a vertex in a specified viewport
		glm::vec2 ScreenToVertex(uint32_t x, uint32_t y, uint32_t screenWidth, uint32_t screenHeight);	
	}
}

#endif