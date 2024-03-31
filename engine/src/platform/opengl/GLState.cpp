#include "GLState.h"
#include "GLHeader.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLState"

namespace pio
{

	uint32_t GetGLBlendEquation(BlendEquation equation)
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
				return GL_NONE;
		}
	}

	uint32_t GetGLBlendFactor(BlendFactor blendFactor)
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
				return GL_NONE;
		}
	}

	uint32_t GetGLFuncAttr(FuncAttr func)
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

	uint32_t GetGLFaceDir(FaceDirection dir)
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

	uint32_t GetGLCullMode(FaceMode mode)
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

	void GLState::SetBlendMode(const Blend &blend)
	{
		if (blend.Enable)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GetGLBlendFactor(blend.Src),
						GetGLBlendFactor(blend.Dst));
			glBlendEquation(GetGLBlendEquation(blend.Equation));
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void GLState::SetClear(const Clear &clear)
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

	void GLState::SetCullFace(const CullFace &cull)
	{
		if (cull.Enable)
		{
			glEnable(GL_CULL_FACE);
			glFrontFace(GetGLFaceDir(cull.Direction));
			glCullFace(GetGLCullMode(cull.Mode));
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}

	void GLState::SetDepthTest(const DepthTest &depth)
	{
		if (depth.Enable)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GetGLFuncAttr(depth.Func));
			glDepthMask(depth.Mark == DepthTest::Mask::ReadWrite ? GL_TRUE : GL_FALSE);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void GLState::SetStencilTest(const StencilTest &stencil)
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
			glStencilFuncSeparate(GL_FRONT, GetGLFuncAttr(stencil.Func(FaceMode_Front).Val), stencil.Func(FaceMode_Front).Ref, stencil.Func(FaceMode_Front).Mask);
			glStencilFuncSeparate(GL_BACK, GetGLFuncAttr(stencil.Func(FaceMode_Back).Val), stencil.Func(FaceMode_Back).Ref, stencil.Func(FaceMode_Back).Mask);
		}
		else
		{
			glStencilFunc(GetGLFuncAttr(stencil.Func(FaceMode_FrontAndBack).Val), stencil.Func(FaceMode_FrontAndBack).Ref, stencil.Func(FaceMode_FrontAndBack).Mask);
		}

		if (stencil.any(StencilSeparateBit_Op))
		{
			glStencilOpSeparate(GL_FRONT, GetGLFuncAttr(stencil.Op(FaceMode_Front).sFail), GetGLFuncAttr(stencil.Op(FaceMode_Front).dpFail), GetGLFuncAttr(stencil.Op(FaceMode_Front).dpPass));
			glStencilOpSeparate(GL_BACK, GetGLFuncAttr(stencil.Op(FaceMode_Back).sFail), GetGLFuncAttr(stencil.Op(FaceMode_Back).dpFail), GetGLFuncAttr(stencil.Op(FaceMode_Back).dpPass));
		}
		else
		{
			glStencilOp(GetGLFuncAttr(stencil.Op(FaceMode_FrontAndBack).sFail), GetGLFuncAttr(stencil.Op(FaceMode_FrontAndBack).dpFail), GetGLFuncAttr(stencil.Op(FaceMode_FrontAndBack).dpPass));
		}

	}
}