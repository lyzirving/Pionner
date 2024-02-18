#include "RenderState.h"

namespace pio
{
	Clear Clear::Common(const glm::vec4 &color)
	{
		Clear clear;
		clear.Color = color;
		clear.Bits.set();
		return clear;
	}

	Clear Clear::Create(ClearBits bit)
	{
		Clear clear;
		clear.Bits.set((uint8_t)bit);
		return clear;
	}

	Clear Clear::Create(uint8_t bits)
	{
		Clear clear;
		if(PIO_BIT((uint8_t)ClearBits::Color) & bits)
			clear.Bits.set((uint8_t)ClearBits::Color);

		if (PIO_BIT((uint8_t)ClearBits::Depth) & bits)
			clear.Bits.set((uint8_t)ClearBits::Depth);

		if (PIO_BIT((uint8_t)ClearBits::Stencil) & bits)
			clear.Bits.set((uint8_t)ClearBits::Stencil);

		return clear;
	}

	Blend Blend::Common()
	{
		Blend blend;
		blend.Enable = true;
		blend.Src = BlendFactor::SrcAlpha;
		blend.Dst = BlendFactor::OneMinusSrcAlpha;
		blend.Equation = BlendEquation::Add;
		return blend;
	}

	Blend Blend::Disable()
	{
		Blend blend;
		blend.Enable = false;
		return blend;
	}

	DepthTest DepthTest::Common()
	{
		DepthTest depthTest;
		depthTest.Enable = true;
		depthTest.Func = FuncAttr::Less;
		depthTest.Mark = DepthTest::Mask::ReadWrite;
		return depthTest;
	}

	DepthTest DepthTest::Always()
	{
		DepthTest depthTest;
		depthTest.Enable = true;
		depthTest.Func = FuncAttr::Always;
		depthTest.Mark = DepthTest::Mask::ReadWrite;
		return depthTest;
	}

	DepthTest DepthTest::Disable()
	{
		DepthTest depthTest;
		depthTest.Enable = false;
		return depthTest;
	}

	CullFace CullFace::Common()
	{
		CullFace cull;
		cull.Enable = true;
		cull.Direction = FaceDirection::CouterClockwise;
		cull.Mode = FaceMode_Back;
		return cull;
	}

	CullFace CullFace::Create(FaceDirection dir, FaceMode mode)
	{
		CullFace cull;
		cull.Enable = true;
		cull.Direction = dir;
		cull.Mode = mode;
		return cull;
	}

	CullFace CullFace::Disable()
	{
		CullFace cull;
		cull.Enable = false;
		return cull;
	}
}