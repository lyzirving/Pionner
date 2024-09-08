#ifndef __PIONNER_BASE_DEFS_TYPE_DEF_H__
#define __PIONNER_BASE_DEFS_TYPE_DEF_H__

#include "MathDef.h"

namespace pio
{
	using byte = uint8_t;

	enum RenderBackendFlags : uint8_t
	{
		RenderBackend_OpenGL = 0,
		RenderBackend_Vulkan,
		RenderBackend_Num
	};

	enum ProjectionType : uint8_t
	{
		ProjectionType_Perspective,
		ProjectionType_Orthographic,
		ProjectionType_Num
	};

	enum class TextureType : uint8_t
	{
		TwoDimen, SingleChannel, 
		NormalMap, RenderBuffer,
		CubeMap, CubeArray,
		Num
	};

	enum class MeshType : uint8_t
	{
		Plane, Cube, Num
	};

	struct Vertex3d
	{
		glm::vec3 Pos{ 0.f };		
		glm::vec2 TexCoord{ 0.f };
		glm::vec3 Normal{ 0.f };
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

		int32_t x() const { return m_x; }
		int32_t y() const { return m_y; }
		int32_t w() const { return m_w; }
		int32_t h() const { return m_h; }

		const glm::vec2 &offset() const { m_offset; }
		const glm::vec2 &ratio() const { m_ratio; }

		float offsetX() const { return m_x + m_offset.x * m_w; }
		float offsetY() const { return m_y + m_offset.y * m_h; }
		float ratioW() const { return m_ratio.x * m_w; }
		float ratioH() const { return m_ratio.y * m_h; }

		void setX(int32_t x) { m_x = x; }
		void setY(int32_t y) { m_y = y; }
		void setW(int32_t w) { m_w = w; }
		void setH(int32_t h) { m_h = h; }

		void setOffset(const glm::vec2 &offset) { m_offset = glm::clamp(offset, glm::vec2(0.f), glm::vec2(1.f)); }
		void setRatio(const glm::vec2 &ratio) { m_ratio = glm::clamp(ratio, glm::vec2(0.f), glm::vec2(1.f)); }

		void reset() { m_offset = glm::vec2(0.f); m_ratio = glm::vec2(1.f); }

		bool operator==(const Viewport& rhs)
		{
			if (this == &rhs) return true;
			return this->m_x == rhs.m_x && this->m_y == rhs.m_y &&
				   this->m_w == rhs.m_w && this->m_h == rhs.m_h &&
				   this->m_offset == rhs.m_offset && this->m_ratio == rhs.m_ratio;
		}

		bool operator!=(const Viewport& rhs) { return !((*this) == rhs); }

	private:
		int32_t m_x{ 0 }, m_y{ 0 };
		int32_t m_w{ 0 }, m_h{ 0 };
		//TODO: click is invalid after the change of offset or ratio
		glm::vec2 m_offset{ 0.f }, m_ratio{ 1.f };
	};

	namespace World
	{
		static const glm::vec3 Right = glm::vec3(1.f, 0.f, 0.f);
		static const glm::vec3 Up = glm::vec3(0.f, 1.f, 0.f);
		static const glm::vec3 Forward = glm::vec3(0.f, 0.f, 1.f);
	}
}

#endif