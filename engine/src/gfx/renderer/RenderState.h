#ifndef __PIONNER_GFX_RENDERER_RENDER_STATE_H__
#define __PIONNER_GFX_RENDERER_RENDER_STATE_H__

#include "core/Base.h"

namespace pio
{
	enum class FuncAttr : uint8_t
	{
		/******* for depth test operation *******/
		Less = 0, Never, Equal, Lequal,
		Greater, Notequal, Gequal, Always,
		/******* for stencil operation *******/
		Keep, Zero, Replace, Incr, IncrWrap,
		Decr, DecrWrap, Invert
	};

	enum ClearBits : uint8_t
	{
		ClearBits_Color = 0, ClearBits_Depth, ClearBits_Stencil, 
		ClearBits_Count
	};	

	enum class BlendFactor : uint8_t
	{
		Zero = 0, One, SrcColor, OneMinusSrcColor, SrcAlpha,
		OneMinusSrcAlpha, DstAlpha, OneMinusDstAlpha, DstColor,
		OneMinusDstColor, SrcAlphaSaturate, ConstantColor,
		OneMinusConstantColor, ConstantAlpha, OneMinusConstantAlpha
	};

	enum class BlendEquation : uint8_t
	{
		Add = 0, Subtract, Reverse_subtract, Min, Max
	};

	enum class RenderMode : uint8_t
	{
		PBR, MaterialPreview, Wireframe, Empty, Num
	};

	struct Clear
	{
		glm::vec4 Color{ 0.f, 0.f, 0.f, 1.f };
		std::bitset<ClearBits_Count> Bits{};

		static Clear Common(const glm::vec4 &color = glm::vec4(0.f, 0.f, 0.f, 1.f));
		static Clear Create(uint8_t flags);
	};

	struct Blend
	{
		bool Enable{ false };
		BlendFactor Src{ BlendFactor::One };
		BlendFactor Dst{ BlendFactor::Zero };
		BlendEquation Equation{ BlendEquation::Add };

		Blend() {}
		Blend(BlendFactor IN_Src, BlendFactor IN_Dst, BlendEquation In_Equation) : Enable(true), Src(IN_Src), Dst(IN_Dst), Equation(In_Equation) {}

		bool operator==(const Blend &rhs);
		bool operator!=(const Blend &rhs) { return !((*this) == rhs); }

		bool operator==(const Blend &rhs) const;
		bool operator!=(const Blend &rhs) const { return !((*this) == rhs); }

		static Blend Common();
		static Blend Disable();
	};

	struct DepthTest
	{
		enum class Mask : uint8_t { ReadWrite = 0, ReadOnly };

		bool Enable{ false };
		FuncAttr Func{ FuncAttr::Less };
		Mask Mark{ Mask::ReadWrite };

		DepthTest() {}
		DepthTest(FuncAttr func, DepthTest::Mask mark) : Enable(true), Func(func), Mark(mark) {}

		bool operator==(const DepthTest &rhs);
		bool operator!=(const DepthTest &rhs) { return !((*this) == rhs); }

		bool operator==(const DepthTest &rhs) const;
		bool operator!=(const DepthTest &rhs) const { return !((*this) == rhs); }

		static DepthTest Common();
		static DepthTest Always();
		static DepthTest Disable();
	};

	enum class FaceDirection : uint8_t
	{
		Clockwise = 0, CouterClockwise
	};

	enum class DrawMode : uint8_t
	{
		Lines, Line_Strip, Line_Loop, Triangles, Triangle_Strip, Triangle_Fan,
	};

	enum FaceMode : uint8_t { FaceMode_Back = 0, FaceMode_Front, FaceMode_FrontAndBack, FaceMode_Num };

	struct CullFace
	{
		bool Enable{ false };
		FaceDirection Direction{ FaceDirection::CouterClockwise };
		FaceMode Mode{ FaceMode_Back };

		bool operator==(const CullFace &rhs);
		bool operator!=(const CullFace &rhs) { return !((*this) == rhs); }

		bool operator==(const CullFace &rhs) const;
		bool operator!=(const CullFace &rhs) const { return !((*this) == rhs); }

		static CullFace Common();
		static CullFace Create(FaceDirection dir, FaceMode mode);
		static CullFace Disable();
	};

	/*
	* @field val: Determines the way how a fragment passes the test.
	* @field ref: Specifies the reference value for the stencil test.
				  The stencil buffer's content is compared to this value.
	* @field mask: Specifies a mask that is ANDed with both the reference value and
	*              the stored stencil value before the test compares them.
	*              Initially set to all 1s.
	*
	* stencilFunc(GL_EQUAL, 1, 0xFF), this tells render API that when the stencil value of a fragment 
	* equals (GFX_EQUAL) the reference value 1, the fragment passes the test and is drawn, otherwise discarded.
	*/
	struct StencilFunc
	{
		FuncAttr Val{ FuncAttr::Always };
		int32_t  Ref{ 1 };
		uint32_t Mask{ 0xff };

		StencilFunc() {}
		StencilFunc(FuncAttr val, int32_t ref, uint32_t mask) : Val(val), Ref(ref), Mask(mask) {}

		bool operator==(const StencilFunc &rhs);
		bool operator!=(const StencilFunc &rhs) { return !((*this) == rhs); }

		bool operator==(const StencilFunc &rhs) const;
		bool operator!=(const StencilFunc &rhs) const { return !((*this) == rhs); }

		static bool ArrayEqual(const StencilFunc *lhs, const StencilFunc *rhs, uint32_t num) { for (size_t i = 0; i < num; i++) { if (lhs[i] != rhs[i]) { return false; } } return true; }
	};

	/*
	* Tell render api how we can actually update the stencil buffer.
	* @field sFail: action to take if the stencil test fails.
	* @field dpFail: action to take if the stencil test passes, but the depth test fails.
	* @field dpPass: action to take if both the stencil and the depth test pass.
	*/
	struct StencilOp
	{
		FuncAttr sFail{ FuncAttr::Keep };
		FuncAttr dpFail{ FuncAttr::Keep };
		FuncAttr dpPass{ FuncAttr::Keep };

		StencilOp() {}
		StencilOp(FuncAttr attr) : sFail(attr), dpFail(attr), dpPass(attr) {}
		StencilOp(FuncAttr in_sFail, FuncAttr in_dpFail, FuncAttr in_dpPass) : sFail(in_sFail), dpFail(in_dpFail), dpPass(in_dpPass) {}

		bool operator==(const StencilOp &rhs);
		bool operator!=(const StencilOp &rhs) { return !((*this) == rhs); }

		bool operator==(const StencilOp &rhs) const;
		bool operator!=(const StencilOp &rhs) const { return !((*this) == rhs); }

		static bool ArrayEqual(StencilOp *lhs, StencilOp *rhs, uint32_t num) { for (size_t i = 0; i < num; i++) { if (lhs[i] != rhs[i]) { return false; } } return true; }
		static bool ArrayEqual(const StencilOp *lhs, const StencilOp *rhs, uint32_t num) { for (size_t i = 0; i < num; i++) { if (lhs[i] != rhs[i]) { return false; } } return true; }
	};

	enum StencilSeparateBit : uint8_t
	{
		StencilSeparateBit_Mask = 0, StencilSeparateBit_Func, StencilSeparateBit_Op, StencilSeparateBit_Num
	};

	struct StencilTest
	{
		bool Enable{ true };

		uint32_t Mask(FaceMode face) { return m_mask[face]; }
		uint32_t Mask(FaceMode face) const { return m_mask[face]; }
		StencilFunc Func(FaceMode face) { return m_func[face]; }
		const StencilFunc &Func(FaceMode face) const { return m_func[face]; }
		StencilOp Op(FaceMode face) { return m_op[face]; }
		const StencilOp &Op(FaceMode face) const { return m_op[face]; }

		void setMask(uint32_t mask) 
		{ 
			m_mask[FaceMode_FrontAndBack] = mask;
			m_separateFlag.reset(StencilSeparateBit_Mask);
		}

		void setMask(uint32_t frontMask, uint32_t backMask) 
		{ 
			m_mask[FaceMode_Front] = frontMask;
			m_mask[FaceMode_Back] = backMask;
			m_separateFlag.set(StencilSeparateBit_Mask);
		}

		void setFunc(const StencilFunc &func) 
		{ 
			m_func[FaceMode_FrontAndBack] = func;
			m_separateFlag.reset(StencilSeparateBit_Func);
		}

		void setFunc(const StencilFunc &frontFaceFunc, const StencilFunc &backFaceFunc)
		{
			m_func[FaceMode_Front] = frontFaceFunc;
			m_func[FaceMode_Back] = backFaceFunc;
			m_separateFlag.set(StencilSeparateBit_Func);
		}

		void setOp(const StencilOp &op) 
		{ 
			m_op[FaceMode_FrontAndBack] = op;
			m_separateFlag.reset(StencilSeparateBit_Op);
		}

		void setOp(const StencilOp &frontFaceOp, const StencilOp &backFaceOp)
		{
			m_op[FaceMode_Front] = frontFaceOp;
			m_op[FaceMode_Back] = backFaceOp;
			m_separateFlag.set(StencilSeparateBit_Op);
		}

		bool any(StencilSeparateBit flag) const { return m_separateFlag.test(flag); }

		StencilTest(bool enbale = true) : Enable(enbale)
		{
			for (uint8_t i = 0; i < FaceMode_Num; i++) { m_mask[i] = 0xff; }
			m_separateFlag.reset();
		}

		bool operator==(const StencilTest &rhs);
		bool operator!=(const StencilTest &rhs) { return !((*this) == rhs); }

	public:
		static StencilTest Common() { return StencilTest(true); }
		static StencilTest Disable() { return StencilTest(false); }		
		
	private:
		uint32_t    m_mask[FaceMode_Num];
		StencilFunc m_func[FaceMode_Num];
		StencilOp   m_op[FaceMode_Num];
		std::bitset<StencilSeparateBit_Num> m_separateFlag;
	};

	struct RenderState
	{
		Clear Clear;
		Blend Blend{ Blend::Common() };
		DepthTest DepthTest{ DepthTest::Common() };
		CullFace Cull{ CullFace::Common() };
		StencilTest Stencil{ StencilTest::Common() };
		RenderMode Mode{ RenderMode::PBR };
		bool Selected{ false };

		RenderState() {}

		RenderState(const pio::Clear &clear, const pio::Blend &blend, const pio::DepthTest &depth, const pio::CullFace &cull, const pio::StencilTest &stencil, pio::RenderMode mode, bool select) 
			: Clear(clear), Blend(blend), DepthTest(depth), Cull(cull), Stencil(stencil), Mode(mode), Selected(select)
		{
		}

		RenderState(const pio::Blend &blend, const pio::DepthTest &depth, const pio::CullFace &cull, const pio::StencilTest &stencil, pio::RenderMode mode)
			: Blend(blend), DepthTest(depth), Cull(cull), Stencil(stencil), Mode(mode)
		{
		}

		RenderState(const pio::Blend &blend, const pio::DepthTest &depth, const pio::CullFace &cull, const pio::StencilTest &stencil)
			: Blend(blend), DepthTest(depth), Cull(cull), Stencil(stencil)
		{
		}

	};
}

#endif