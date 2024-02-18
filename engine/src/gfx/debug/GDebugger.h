#ifndef __PIONNER_GFX_DEBUG_GDEBUGGER_H__
#define __PIONNER_GFX_DEBUG_GDEBUGGER_H__

#include "core/math/Ray.h"
#include "gfx/struct/Geometry2D.h"

namespace pio
{
	enum class GDebugType : uint8_t
	{
		Line = 0, Num
	};

	class GDebugger
	{
		PIO_SINGLETON_DECLARE(GDebugger)
	public:
		void drawLine(const Ray &r, float len, const glm::vec4 &color);
		void drawLine(const glm::vec3 &start, const glm::vec3 &end, const glm::vec4 &color);

		bool any(GDebugType type);
		void clear(GDebugType type);		
		void flush();

	public:
		inline Ref<LineMesh> getLineMesh() { return m_lineMesh; }

	private:
		Ref<LineMesh> m_lineMesh;
		std::bitset<PIO_UINT8(GDebugType::Num)> m_dirty;
	};
}

#endif