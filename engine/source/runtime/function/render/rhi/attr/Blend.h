#ifndef __RHI_BLEND_H__
#define __RHI_BLEND_H__

#include <cstdint>

namespace Pionner
{
	enum BlendFactor : uint8_t
	{
		Bld_Factor_Zero,
		Bld_Factor_One,
		Bld_Factor_SrcColor,
		Bld_Factor_OneMinusSrcColor,
		Bld_Factor_SrcAlpha,
		Bld_Factor_OneMinusSrcAlpha,
		Bld_Factor_DstAlpha,
		Bld_Factor_OneMinusDstAlpha,
		Bld_Factor_DstColor,
		Bld_Factor_OneMinusDstColor,
		Bld_Factor_SrcAlphaSaturate,
		Bld_Factor_ConstantColor,
		Bld_Factor_OneMinusConstantColor,
		Bld_Factor_ConstantAlpha,
		Bld_Factor_OneMinusConstantAlpha
	};

	class Blend
	{
	public:
		Blend() : m_enable(false),
			m_src(Bld_Factor_Zero), m_dest(Bld_Factor_Zero)
		{
		};

		~Blend() = default;

		static Blend disable()
		{
			Blend blend;
			blend.m_enable = false;
			return blend;
		}

		static Blend common()
		{
			Blend blend;
			blend.m_enable = true;
			blend.m_src = Bld_Factor_SrcAlpha;
			blend.m_dest = Bld_Factor_OneMinusSrcAlpha;
			return blend;
		}

	public:
		bool m_enable;
		BlendFactor m_src, m_dest;
	};
}

#endif