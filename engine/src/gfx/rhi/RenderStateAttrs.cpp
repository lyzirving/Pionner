#include "RenderStateAttrs.h"

namespace pio
{
	bool Clear::operator==(const Clear& rhs) const
	{
		return Color == rhs.Color && Bits == rhs.Bits;
	}

	Clear Clear::Common(const glm::vec4& color)
	{
		Clear clear;
		clear.Color = color;
		clear.Bits.set();
		return clear;
	}

	Clear Clear::Create(uint8_t bits)
	{
		Clear clear;
		if (PIO_BIT_MOV(ClearBits_Color) & bits)
			clear.Bits.set(ClearBits_Color);

		if (PIO_BIT_MOV(ClearBits_Depth) & bits)
			clear.Bits.set(ClearBits_Depth);

		if (PIO_BIT_MOV(ClearBits_Stencil) & bits)
			clear.Bits.set(ClearBits_Stencil);

		return clear;
	}

	Clear Clear::Disable()
	{
		Clear clear;
		clear.Bits.reset();
		return clear;
	}

	bool Blend::operator==(const Blend& rhs) const
	{
		if (this == &rhs)
			return true;

		if (this->Enable != rhs.Enable)
			return false;

		return this->Enable && this->Src == rhs.Src && this->Dst == rhs.Dst && this->Equation == rhs.Equation;
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

	bool DepthTest::operator==(const DepthTest& rhs) const
	{
		if (this == &rhs)
			return true;

		if (this->Enable != rhs.Enable)
			return false;

		return this->Enable && this->Func == rhs.Func && this->Mark == rhs.Mark;
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

	bool CullFace::operator==(const CullFace& rhs) const
	{
		if (this == &rhs)
			return true;

		if (this->Enable != rhs.Enable)
			return false;

		return this->Enable && this->Direction == rhs.Direction && this->Mode == rhs.Mode;
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

	bool StencilFunc::operator==(const StencilFunc& rhs) const
	{
		if (this == &rhs)
			return true;

		return this->Val == rhs.Val && this->Ref == rhs.Ref && this->Mask == rhs.Mask;
	}

	bool StencilOp::operator==(const StencilOp& rhs) const
	{
		if (this == &rhs)
			return true;

		return this->dpFail == rhs.dpFail && this->dpPass == rhs.dpPass && this->sFail == rhs.sFail;
	}

	bool StencilTest::operator==(const StencilTest& rhs) const
	{
		if (this == &rhs)
			return true;

		if (this->Enable != rhs.Enable)
			return false;

		return this->Enable && (std::memcmp(this->m_Mask, rhs.m_Mask, FaceMode_Num * sizeof(uint32_t)) == 0) &&
			this->m_SeparateFlag == rhs.m_SeparateFlag &&
			StencilFunc::ArrayEqual(this->m_Func, rhs.m_Func, FaceMode_Num) &&
			StencilOp::ArrayEqual(this->m_Op, rhs.m_Op, FaceMode_Num);
	}
}