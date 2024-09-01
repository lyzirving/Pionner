#include "GLRenderState.h"

#include "GLHeader.h"
#include "GLHelper.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLRenderState"

namespace pio
{
	uint32_t GetBlendEquation(BlendEquation equation)
	{
		switch (equation)
		{
			case BlendEquation::Add:
				return GL_FUNC_ADD;
			case BlendEquation::Subtract:
				return GL_FUNC_SUBTRACT;
			case BlendEquation::Reverse_subtract:
				return GL_FUNC_REVERSE_SUBTRACT;
			case BlendEquation::Min:
				return GL_MIN;
			case BlendEquation::Max:
				return GL_MAX;
			default:
				LOGE("invalid blend equation[%u]", equation);
				std::abort();
				return GL_NONE;
		}
	}

	uint32_t GetBlendFactor(BlendFactor blendFactor)
	{
		switch (blendFactor)
		{
			case BlendFactor::Zero:
				return GL_ZERO;
			case BlendFactor::One:
				return GL_ONE;
			case BlendFactor::SrcColor:
				return GL_SRC_COLOR;
			case BlendFactor::OneMinusSrcColor:
				return GL_ONE_MINUS_SRC_COLOR;
			case BlendFactor::SrcAlpha:
				return GL_SRC_ALPHA;
			case BlendFactor::OneMinusSrcAlpha:
				return GL_ONE_MINUS_SRC_ALPHA;
			case BlendFactor::DstAlpha:
				return GL_DST_ALPHA;
			case BlendFactor::OneMinusDstAlpha:
				return GL_ONE_MINUS_DST_ALPHA;
			case BlendFactor::DstColor:
				return GL_DST_COLOR;
			case BlendFactor::OneMinusDstColor:
				return GL_ONE_MINUS_DST_COLOR;
			case BlendFactor::SrcAlphaSaturate:
				return GL_SRC_ALPHA_SATURATE;
			case BlendFactor::ConstantColor:
				return GL_CONSTANT_COLOR;
			case BlendFactor::OneMinusConstantColor:
				return GL_ONE_MINUS_CONSTANT_COLOR;
			case BlendFactor::ConstantAlpha:
				return GL_CONSTANT_ALPHA;
			case BlendFactor::OneMinusConstantAlpha:
				return GL_ONE_MINUS_CONSTANT_ALPHA;
			default:
				LOGE("invalid blend factor[%u]", blendFactor);
				std::abort();
				return GL_NONE;
		}
	}

	uint32_t GetFaceDir(FaceDirection dir)
	{
		switch (dir)
		{
			case FaceDirection::Clockwise:
				return GL_CW;
			case FaceDirection::CouterClockwise:
			default:
				return GL_CCW;
		}
	}

	uint32_t GetCullMode(FaceMode mode)
	{
		switch (mode)
		{
			case FaceMode_Front:
				return GL_FRONT;
			case FaceMode_FrontAndBack:
				return GL_FRONT_AND_BACK;
			case FaceMode_Back:
			default:
				return GL_BACK;
		}
	}

	uint32_t GetFuncAttr(FuncAttr func)
	{
		switch (func)
		{
			case FuncAttr::Never:
				return GL_NEVER;
			case FuncAttr::Equal:
				return GL_EQUAL;
			case FuncAttr::Lequal:
				return GL_LEQUAL;
			case FuncAttr::Greater:
				return GL_GREATER;
			case FuncAttr::Notequal:
				return GL_NOTEQUAL;
			case FuncAttr::Gequal:
				return GL_GEQUAL;
			case FuncAttr::Always:
				return GL_ALWAYS;
			case FuncAttr::Keep:
				return GL_KEEP;
			case FuncAttr::Zero:
				return GL_ZERO;
			case FuncAttr::Replace:
				return GL_REPLACE;
			case FuncAttr::Incr:
				return GL_INCR;
			case FuncAttr::IncrWrap:
				return GL_INCR_WRAP;
			case FuncAttr::Decr:
				return GL_DECR;
			case FuncAttr::DecrWrap:
				return GL_DECR_WRAP;
			case FuncAttr::Invert:
				return GL_INVERT;
			case FuncAttr::Less:
			default:
				return GL_LESS;
		}
	}

	GLRenderState::GLRenderState(RenderBackendFlags flag) : RenderState(flag)
	{
	}

	void GLRenderState::setClear(const Clear& clear)
	{
		uint32_t bits{ 0 };
		if (clear.Bits.test(ClearBits_Color))
			bits |= GL_COLOR_BUFFER_BIT;

		if (clear.Bits.test(ClearBits_Depth))
			bits |= GL_DEPTH_BUFFER_BIT;

		if (clear.Bits.test(ClearBits_Stencil))
			bits |= GL_STENCIL_BUFFER_BIT;

		glClearColor(clear.Color.r, clear.Color.g, clear.Color.b, clear.Color.a);
		glClear(bits);
	}

	void GLRenderState::setCullFace(const CullFace& cull)
	{
		if (cull.Enable)
		{
			glEnable(GL_CULL_FACE);
			glFrontFace(GetFaceDir(cull.Direction));
			glCullFace(GetCullMode(cull.Mode));
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}

	void GLRenderState::setBlendMode(const Blend& blend)
	{
		if (blend.Enable)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GetBlendFactor(blend.Src),
						GetBlendFactor(blend.Dst));
			glBlendEquation(GetBlendEquation(blend.Equation));
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void GLRenderState::setDepthTest(const DepthTest& depth)
	{
		if (depth.Enable)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GetFuncAttr(depth.Func));
			glDepthMask(depth.Mark == DepthTest::Mask::ReadWrite ? GL_TRUE : GL_FALSE);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void GLRenderState::setStencilTest(const StencilTest& stencil)
	{
		if (!stencil.Enable)
		{
			glDisable(GL_STENCIL_TEST);
			return;
		}
		glEnable(GL_STENCIL_TEST);

		if (stencil.any(StencilSeparateBit_Mask))
		{
			glStencilMaskSeparate(GL_FRONT, stencil.Mask(FaceMode_Front));
			glStencilMaskSeparate(GL_BACK, stencil.Mask(FaceMode_Back));
		}
		else
		{
			glStencilMask(stencil.Mask(FaceMode_FrontAndBack));
		}

		if (stencil.any(StencilSeparateBit_Func))
		{
			glStencilFuncSeparate(GL_FRONT, GetFuncAttr(stencil.Func(FaceMode_Front).Val), stencil.Func(FaceMode_Front).Ref, stencil.Func(FaceMode_Front).Mask);
			glStencilFuncSeparate(GL_BACK, GetFuncAttr(stencil.Func(FaceMode_Back).Val), stencil.Func(FaceMode_Back).Ref, stencil.Func(FaceMode_Back).Mask);
		}
		else
		{
			glStencilFunc(GetFuncAttr(stencil.Func(FaceMode_FrontAndBack).Val), stencil.Func(FaceMode_FrontAndBack).Ref, stencil.Func(FaceMode_FrontAndBack).Mask);
		}

		if (stencil.any(StencilSeparateBit_Op))
		{
			glStencilOpSeparate(GL_FRONT, GetFuncAttr(stencil.Op(FaceMode_Front).sFail), GetFuncAttr(stencil.Op(FaceMode_Front).dpFail), GetFuncAttr(stencil.Op(FaceMode_Front).dpPass));
			glStencilOpSeparate(GL_BACK, GetFuncAttr(stencil.Op(FaceMode_Back).sFail), GetFuncAttr(stencil.Op(FaceMode_Back).dpFail), GetFuncAttr(stencil.Op(FaceMode_Back).dpPass));
		}
		else
		{
			glStencilOp(GetFuncAttr(stencil.Op(FaceMode_FrontAndBack).sFail), GetFuncAttr(stencil.Op(FaceMode_FrontAndBack).dpFail), GetFuncAttr(stencil.Op(FaceMode_FrontAndBack).dpPass));
		}
	}
}