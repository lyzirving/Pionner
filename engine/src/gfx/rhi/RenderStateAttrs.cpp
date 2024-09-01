#include "RenderStateAttrs.h"

namespace pio
{
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
		if (PIO_BIT(ClearBits_Color) & bits)
			clear.Bits.set(ClearBits_Color);

		if (PIO_BIT(ClearBits_Depth) & bits)
			clear.Bits.set(ClearBits_Depth);

		if (PIO_BIT(ClearBits_Stencil) & bits)
			clear.Bits.set(ClearBits_Stencil);

		return clear;
	}

	bool Blend::operator==(const Blend& rhs)
	{
		if (this == &rhs)
			return true;

		if (this->Enable != rhs.Enable)
			return false;

		return this->Enable && this->Src == rhs.Src && this->Dst == rhs.Dst && this->Equation == rhs.Equation;
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

	bool DepthTest::operator==(const DepthTest& rhs)
	{
		if (this == &rhs)
			return true;

		if (this->Enable != rhs.Enable)
			return false;

		return this->Enable && this->Func == rhs.Func && this->Mark == rhs.Mark;
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

	bool CullFace::operator==(const CullFace& rhs)
	{
		if (this == &rhs)
			return true;

		if (this->Enable != rhs.Enable)
			return false;

		return this->Enable && this->Direction == rhs.Direction && this->Mode == rhs.Mode;
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

	bool StencilFunc::operator==(const StencilFunc& rhs)
	{
		if (this == &rhs)
			return true;

		return this->Val == rhs.Val && this->Ref == rhs.Ref && this->Mask == rhs.Mask;
	}

	bool StencilFunc::operator==(const StencilFunc& rhs) const
	{
		if (this == &rhs)
			return true;

		return this->Val == rhs.Val && this->Ref == rhs.Ref && this->Mask == rhs.Mask;
	}

	bool StencilOp::operator==(const StencilOp& rhs)
	{
		if (this == &rhs)
			return true;

		return this->dpFail == rhs.dpFail && this->dpPass == rhs.dpPass && this->sFail == rhs.sFail;
	}

	bool StencilOp::operator==(const StencilOp& rhs) const
	{
		if (this == &rhs)
			return true;

		return this->dpFail == rhs.dpFail && this->dpPass == rhs.dpPass && this->sFail == rhs.sFail;
	}

	bool StencilTest::operator==(const StencilTest& rhs)
	{
		if (this == &rhs)
			return true;

		if (this->Enable != rhs.Enable)
			return false;

		return this->Enable && (std::memcmp(this->m_mask, rhs.m_mask, FaceMode_Num * sizeof(uint32_t)) == 0) &&
			this->m_separateFlag == rhs.m_separateFlag &&
			StencilFunc::ArrayEqual(this->m_func, rhs.m_func, FaceMode_Num) &&
			StencilOp::ArrayEqual(this->m_op, rhs.m_op, FaceMode_Num);
	}
}