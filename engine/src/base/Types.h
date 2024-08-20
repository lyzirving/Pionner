#ifndef __PIONNER_BASE_TYPES_H__
#define __PIONNER_BASE_TYPES_H__

#include "Math.h"

namespace pio
{
	enum BackendFlags : uint8_t
	{
		Backend_OpenGL = 0,
		Backend_Vulkan,
		Backend_Num
	};

	enum ProjectionType : uint8_t
	{
		ProjectionType_Perspective,
		ProjectionType_Orthographic,
		ProjectionType_Num
	};

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
}

#endif