#pragma once

#include "gfx/GfxDef.h"

namespace pio
{
	enum class FuncAttr : uint8_t
	{
		/******* for depth test operation *******/
		Less, Never, Equal, Lequal,
		Greater, Notequal, Gequal, Always,
		/******* for stencil operation *******/
		Keep, Zero, Replace, Incr, IncrWrap,
		Decr, DecrWrap, Invert
	};

	enum ClearBits : uint8_t
	{
		ClearBits_Color, ClearBits_Depth, ClearBits_Stencil, ClearBits_Count
	};

	enum class BlendFactor : uint8_t
	{
		Zero, One, SrcColor, OneMinusSrcColor, SrcAlpha,
		OneMinusSrcAlpha, DstAlpha, OneMinusDstAlpha, DstColor,
		OneMinusDstColor, SrcAlphaSaturate, ConstantColor,
		OneMinusConstantColor, ConstantAlpha, OneMinusConstantAlpha
	};

	enum class BlendEquation : uint8_t
	{
		Add, Subtract, Reverse_subtract, Min, Max
	};

	enum class FaceDirection : uint8_t
	{
		Clockwise, CouterClockwise
	};

	enum FaceMode : uint8_t 
	{ 
		FaceMode_Back, FaceMode_Front, FaceMode_FrontAndBack, FaceMode_Num 
	};

	enum StencilSeparateBit : uint8_t
	{
		StencilSeparateBit_Mask, StencilSeparateBit_Func, StencilSeparateBit_Op, StencilSeparateBit_Num
	};

	struct Clear
	{
		glm::vec4 Color{ 0.f, 0.f, 0.f, 1.f };
		std::bitset<ClearBits_Count> Bits{};

		Clear& SetColor(const glm::vec4& color) { Color = color; return *this; }
		Clear& SetColorBit(bool set = true) { if(set) { Bits.set(ClearBits_Color); } else { Bits.reset(ClearBits_Color); }  return *this; }
		Clear& SetDepthBit(bool set = true) { if(set) { Bits.set(ClearBits_Depth); } else { Bits.reset(ClearBits_Depth); }  return *this; }
		Clear& SetStencilBit(bool set = true) { if(set) { Bits.set(ClearBits_Stencil); } else { Bits.reset(ClearBits_Stencil); }  return *this; }

		bool operator==(const Clear& rhs) const;
		bool operator!=(const Clear& rhs) const { return !((*this) == rhs); }

		static Clear Common(const glm::vec4& color = glm::vec4(0.f, 0.f, 0.f, 1.f));
		static Clear Create(uint8_t flags);
		static Clear Disable();
	};

	struct Blend
	{
		bool Enable{ false };
		BlendFactor Src{ BlendFactor::One };
		BlendFactor Dst{ BlendFactor::Zero };
		BlendEquation Equation{ BlendEquation::Add };

		Blend() {}
		Blend(BlendFactor IN_Src, BlendFactor IN_Dst, BlendEquation In_Equation) : Enable(true), Src(IN_Src), Dst(IN_Dst), Equation(In_Equation) {}

		bool operator==(const Blend& rhs) const;
		bool operator!=(const Blend& rhs) const { return !((*this) == rhs); }

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

		bool operator==(const DepthTest& rhs) const;
		bool operator!=(const DepthTest& rhs) const { return !((*this) == rhs); }

		static DepthTest Common();
		static DepthTest Always();
		static DepthTest Disable();
	};

	struct CullFace
	{
		bool Enable{ false };
		FaceDirection Direction{ FaceDirection::CouterClockwise };
		FaceMode Mode{ FaceMode_Back };

		bool operator==(const CullFace& rhs) const;
		bool operator!=(const CullFace& rhs) const { return !((*this) == rhs); }

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

		bool operator==(const StencilFunc& rhs) const;
		bool operator!=(const StencilFunc& rhs) const { return !((*this) == rhs); }

		static bool ArrayEqual(const StencilFunc* lhs, const StencilFunc* rhs, uint32_t num) { for (size_t i = 0; i < num; i++) { if (lhs[i] != rhs[i]) { return false; } } return true; }
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

		bool operator==(const StencilOp& rhs) const;
		bool operator!=(const StencilOp& rhs) const { return !((*this) == rhs); }

		static bool ArrayEqual(StencilOp* lhs, StencilOp* rhs, uint32_t num) { for (size_t i = 0; i < num; i++) { if (lhs[i] != rhs[i]) { return false; } } return true; }
		static bool ArrayEqual(const StencilOp* lhs, const StencilOp* rhs, uint32_t num) { for (size_t i = 0; i < num; i++) { if (lhs[i] != rhs[i]) { return false; } } return true; }
	};

	struct StencilTest
	{
		bool Enable{ true };

		uint32_t Mask(FaceMode face) { return m_Mask[face]; }
		uint32_t Mask(FaceMode face) const { return m_Mask[face]; }
		StencilFunc Func(FaceMode face) { return m_Func[face]; }
		const StencilFunc& Func(FaceMode face) const { return m_Func[face]; }
		StencilOp Op(FaceMode face) { return m_Op[face]; }
		const StencilOp& Op(FaceMode face) const { return m_Op[face]; }

		void SetMask(uint32_t mask)
		{
			m_Mask[FaceMode_FrontAndBack] = mask;
			m_SeparateFlag.reset(StencilSeparateBit_Mask);
		}

		void SetMask(uint32_t frontMask, uint32_t backMask)
		{
			m_Mask[FaceMode_Front] = frontMask;
			m_Mask[FaceMode_Back] = backMask;
			m_SeparateFlag.set(StencilSeparateBit_Mask);
		}

		void SetFunc(const StencilFunc& func)
		{
			m_Func[FaceMode_FrontAndBack] = func;
			m_SeparateFlag.reset(StencilSeparateBit_Func);
		}

		void SetFunc(const StencilFunc& frontFaceFunc, const StencilFunc& backFaceFunc)
		{
			m_Func[FaceMode_Front] = frontFaceFunc;
			m_Func[FaceMode_Back] = backFaceFunc;
			m_SeparateFlag.set(StencilSeparateBit_Func);
		}

		void SetOp(const StencilOp& op)
		{
			m_Op[FaceMode_FrontAndBack] = op;
			m_SeparateFlag.reset(StencilSeparateBit_Op);
		}

		void SetOp(const StencilOp& frontFaceOp, const StencilOp& backFaceOp)
		{
			m_Op[FaceMode_Front] = frontFaceOp;
			m_Op[FaceMode_Back] = backFaceOp;
			m_SeparateFlag.set(StencilSeparateBit_Op);
		}

		bool Any(StencilSeparateBit flag) const { return m_SeparateFlag.test(flag); }

		StencilTest(bool enbale = true) : Enable(enbale)
		{
			for (uint8_t i = 0; i < FaceMode_Num; i++) { m_Mask[i] = 0xff; }
			m_SeparateFlag.reset();
		}

		bool operator==(const StencilTest& rhs) const;
		bool operator!=(const StencilTest& rhs) const { return !((*this) == rhs); }

	public:
		static StencilTest Common() { return StencilTest(true); }
		static StencilTest Disable() { return StencilTest(false); }

	private:
		uint32_t    m_Mask[FaceMode_Num];
		StencilFunc m_Func[FaceMode_Num];
		StencilOp   m_Op[FaceMode_Num];
		std::bitset<StencilSeparateBit_Num> m_SeparateFlag;
	};

	struct RenderStateAttrs
	{
		Clear AttrClear{};
		CullFace AttrCull{};
		Blend AttrBlend{};
		DepthTest AttrDepth{};
		StencilTest AttrStencil{};

		RenderStateAttrs &SetClear(const Clear &val) { AttrClear = val; return *this; }
		RenderStateAttrs &SetCull(const CullFace &val) { AttrCull = val; return *this; }
		RenderStateAttrs &SetBlend(const Blend &val) { AttrBlend = val; return *this; }
		RenderStateAttrs &SetDepth(const DepthTest &val) { AttrDepth = val; return *this; }
		RenderStateAttrs &SetStencil(const StencilTest &val) { AttrStencil = val; return *this; }
	};
}