#ifndef __PIONNER_UI_DEF_H__
#define __PIONNER_UI_DEF_H__

#include "Common.h"

namespace pio
{
	/*
	* @brief: Rectangle of a panel on window whose origin is left-top
	*/
	struct Rect2d
	{
		uint32_t Left{ 0 };
		uint32_t Top{ 0 };
		uint32_t Right{ 0 };
		uint32_t Bottom{ 0 };

		Rect2d() {}
		Rect2d(uint32_t l, uint32_t t, uint32_t r, uint32_t b) : Left(l), Top(t), Right(r), Bottom(b) {}
		Rect2d(const glm::ivec2 lt, uint32_t width, uint32_t height) : Left(lt.x), Top(lt.y), Right(lt.x + width), Bottom(lt.y + height) {}

		uint32_t width()  const { return Right - Left; }
		uint32_t height() const { return Bottom - Top; }
		glm::vec2 center() const { return glm::vec2(Left + width() * 0.5f, Top + height() * 0.5f); }
		bool contain(uint32_t x, uint32_t y) const { return x >= Left && x <= Right && y >= Top && y <= Bottom; }	

		bool operator==(const Rect2d& rhs)
		{
			if (this == &rhs) return true;
			return this->Left == rhs.Left && this->Top == rhs.Top && this->Right == rhs.Right && this->Bottom == rhs.Bottom;
		}

		bool operator!=(const Rect2d& rhs) { return !((*this) == rhs); }

		bool bIntersect(const Rect2d& rhs)
		{
			glm::uvec2 lt = glm::uvec2(std::max(Left, rhs.Left), std::max(Top, rhs.Top));
			glm::uvec2 rb = glm::uvec2(std::min(Right, rhs.Right), std::min(Bottom, rhs.Bottom));
			return lt.x <= rb.x && lt.y <= rb.y;
		}

		void doUnion(const Rect2d& rhs)
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

	struct LayoutRatio
	{
		float Left{ 0.f };  // 0.f - 1.f
		float Top{ 0.f };   // 0.f - 1.f
		float Right{ 1.f }; // 0.f - 1.f
		float Bottom{ 1.f };// 0.f - 1.f

		LayoutRatio() {}
		LayoutRatio(float l, float t, float r, float b)
			: Left(Math::Clamp(l, 0.f, 1.f)), Top(Math::Clamp(t, 0.f, 1.f))
			, Right(Math::Clamp(r, 0.f, 1.f)), Bottom(Math::Clamp(b, 0.f, 1.f))
		{
		}
	};

	/*
	* @brief: LayoutParams is mainly used for layout on the entire window.
	*		  LayoutParams.Ratio defines the rect on window by ratio in [0, 1].
	*		  LayoutParams.Rect is the pixel result of Ratio with origin on the left-top.
	*/
	struct LayoutParams
	{
		LayoutRatio Ratio;//Layout ratio on entire window
		Rect2d Rect; //Position on entire window

		LayoutParams() {}

		LayoutParams(uint32_t w, uint32_t h, float l, float t, float r, float b) : Ratio(l, t, r, b), Rect() 
		{
			calculate(w, h);
		}

		void calculate(uint32_t w, uint32_t h)
		{
			Rect.Left = w * Ratio.Left;
			Rect.Top = h * Ratio.Top;
			Rect.Right = w * Ratio.Right;
			Rect.Bottom = h * Ratio.Bottom;
		}
	};
}

#endif