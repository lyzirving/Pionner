#ifndef __PIONNER_GFX_DEBUG_GDEBUGGER_H__
#define __PIONNER_GFX_DEBUG_GDEBUGGER_H__

#include "core/math/Ray.h"
#include "gfx/struct/Geometry2D.h"

namespace pio
{
	class UniformBufferSet;
	struct RenderState;
	
	enum GDebugType : uint8_t
	{
		GDebug_Line, GDebug_Num
	};

	class GDebugger
	{
		PIO_SINGLETON_DECLARE(GDebugger)
	public:
		void addLine(const Ray &r, float len = 30.f);
		void addLine(const glm::vec3 &start, const glm::vec3 &end);
		void clear(GDebugType type);

		void drawLine(const Ref<UniformBufferSet> &ubs, const RenderState &state);		

	private:
		bool any(GDebugType type);				
		void flush(GDebugType type);

	public:
		static glm::vec4 Color;

	private:
		Ref<LineSegment> m_lines;
		std::bitset<GDebug_Num> m_dirty;
	};
}

#endif