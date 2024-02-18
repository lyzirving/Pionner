#ifndef __PIONNER_UI_UIDEF_H__
#define __PIONNER_UI_UIDEF_H__

#include "asset/Asset.h"

namespace pio
{

	#define CLICK_INTERVAL (300)

	constexpr static const char *STR_AXIS_X = "AxisX";
	constexpr static const char *STR_AXIS_Y = "AxisY";
	constexpr static const char *STR_AXIS_Z = "AxisZ";

	enum class CoordinateShape : uint8_t
	{
		Cylinder, Arrow, Num
	};

	struct UIEventTracker
	{
		bool ButtonPressed{ false };
		uint64_t PressedTime{ 0 };// ms
		glm::vec2 Cursor{ -1.f };
		glm::vec2 LastCursor{ -1.f };

		static bool IsClick(uint64_t now, uint64_t pre) { return (now - pre) <= CLICK_INTERVAL; }
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

	struct LayoutRect
	{
		uint32_t Left{ 0 };
		uint32_t Top{ 0 };
		uint32_t Right{ 0 };
		uint32_t Bottom{ 0 };

		inline uint32_t width() const { return Right - Left; }
		inline uint32_t height() const { return Bottom - Top; }
		inline bool contain(uint32_t x, uint32_t y) const { return x >= Left && x <= Right && y >= Top && y <= Bottom; }
	};

	struct LayoutViewport
	{
		uint32_t X{ 0 };
		uint32_t Y{ 0 };
		uint32_t Width{ 0 };
		uint32_t Height{ 0 };
	};

	struct LayoutParams
	{
		LayoutPercentage Percentage;
		LayoutRect Position;
		LayoutViewport Viewport;

		LayoutParams() : Percentage(), Position(), Viewport()
		{
		}

		LayoutParams(float l, float t, float r, float b) 
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
		glm::uvec2 ScreenToViewport(const glm::vec2 &screenPt, const LayoutParams& param);
		glm::vec2 ScreenToVertex(uint32_t x, uint32_t y, uint32_t screenWidth, uint32_t screenHeight);
	}
}

#endif