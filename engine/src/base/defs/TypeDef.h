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

	struct Vertex3d
	{
		glm::vec3 Pos{ 0.f };		
		glm::vec2 TexCoord{ 0.f };
		glm::vec3 Normal{ 0.f };

		Vertex3d() {}
		Vertex3d(const glm::vec3& pos) : Pos(pos) {}
		Vertex3d(const glm::vec3& pos, const glm::vec2& texCoord) : Pos(pos), TexCoord(texCoord) {}
		Vertex3d(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal) : Pos(pos), TexCoord(texCoord), Normal(normal) {}
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