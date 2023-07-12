#ifndef __PIONNER_GFX_CONTEXT_ATTR_CLEAR_MODE_H__
#define __PIONNER_GFX_CONTEXT_ATTR_CLEAR_MODE_H__

#include <glm/glm.hpp>
#include <bitset>

namespace pio
{
	namespace gfx
	{
		enum ClearModeBit : uint8_t
		{
			CLEAR_MOD_COLOR_BIT,
			CLEAR_MOD_DEPTH_BIT,
			CLEAR_MOD_CNT
		};

		class ClearMode
		{
		public:
			ClearMode() {};
			~ClearMode() = default;

			static ClearMode common(const glm::vec4 color)
			{
				ClearMode mode;
				mode.m_color = color;
				mode.m_bits.set(CLEAR_MOD_COLOR_BIT);
				mode.m_bits.set(CLEAR_MOD_DEPTH_BIT);
				return mode;
			}

		public:
			glm::vec4 m_color{ 0.f };
			std::bitset<CLEAR_MOD_CNT> m_bits{};
		};
	}
}

#endif