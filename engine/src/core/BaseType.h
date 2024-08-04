#ifndef __PIONNER_CORE_BASE_TYPE_H__
#define __PIONNER_CORE_BASE_TYPE_H__

#include "core/math/MathLib.h"

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

	/*
	* @brief: Render area on window whose origin is left-bottom
	*/
	struct Viewport
	{
		Viewport() {}
		Viewport(int32_t x, int32_t y, int32_t w, int32_t h) : m_x(x), m_y(y), m_w(w), m_h(h) {}

		int32_t x() const { return m_x + m_offsetX * m_w; }
		int32_t y() const { return m_y + m_offsetY * m_h; }
		int32_t w() const { return m_ratioW * m_w; }
		int32_t h() const { return m_ratioH * m_h; }

		float offsetX() const { return m_offsetX; }
		float offsetY() const { return m_offsetY; }
		float ratioW() const  { return m_ratioW; }
		float ratioH() const  { return m_ratioH; }

		void setX(int32_t x) { m_x = x; }
		void setY(int32_t y) { m_y = y; }
		void setW(int32_t w) { m_w = w; }
		void setH(int32_t h) { m_h = h; }

		void setOffsetX(float x) { m_offsetX = Math::Clamp(x, 0.f, 1.f); }
		void setOffsetY(float y) { m_offsetY = Math::Clamp(y, 0.f, 1.f); }
		void setRatioW(float r)  { m_ratioW = Math::Clamp(r, 0.f, 1.f); }
		void setRatioH(float r)  { m_ratioH = Math::Clamp(r, 0.f, 1.f); }

		void reset() { m_offsetX = m_offsetY = 0.f; m_ratioW = m_ratioH = 1.f; }

		bool operator==(const Viewport& rhs)
		{
			if (this == &rhs) return true;
			return this->m_x == rhs.m_x && this->m_y == rhs.m_y && 
				   this->m_w == rhs.m_w && this->m_h == rhs.m_h && 
				   this->m_offsetX == rhs.m_offsetX && this->m_offsetY == rhs.m_offsetY &&
				   this->m_ratioW == rhs.m_ratioW && this->m_ratioH == rhs.m_ratioH;
		}

		bool operator!=(const Viewport& rhs) { return !((*this) == rhs); }

	private:
		int32_t m_x{ 0 }, m_y{ 0 };
		int32_t m_w{ 0 }, m_h{ 0 };
		//TODO: click is invalid after the change of offset or ratio
		float m_offsetX{ 0.f }, m_offsetY{ 0.f};
		float m_ratioW{ 1.f }, m_ratioH{ 1.f };
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
	*		  LayoutParams.Position is the pixel result of Ratio with origin on the left-top.
	*		  LayoutParams.Viewport is the rendering viewport setup by this panel, whose origin is the left-bottom of panel.
	*/
	struct LayoutParams
	{
		LayoutRatio Ratio;//Layout ratio on entire window
		Rect2d Position;//Position on entire window
		Viewport Viewport;// Rendering panel on window

		LayoutParams() {}

		LayoutParams(float l, float t, float r, float b) : Ratio(l, t, r, b), Position(), Viewport() {}

		void calculate(uint32_t wid, uint32_t height)
		{
			Position.Left = wid * Ratio.Left;
			Position.Top = height * Ratio.Top;
			Position.Right = wid * Ratio.Right;
			Position.Bottom = height * Ratio.Bottom;

			Viewport.setW(Position.Right - Position.Left);
			Viewport.setH(Position.Bottom - Position.Top);
			Viewport.setX(Position.Left);
			Viewport.setY(height - Position.Top - Viewport.h());
		}
	};
}

#endif