#include "State.h"

#include "render/rhi/RhiHeader.h"

namespace pio
{
	namespace gfx
	{
		uint32_t getGLFaceDir(FaceDirection dir)
		{
			switch (dir)
			{
				case CLOCK_WISE:
					return GL_CW;
				case COUNTER_CLOCK_WISE:
					return GL_CCW;
				default:
					return GL_FALSE;
			}
		}

		uint32_t getGLCullMode(CullMode mode)
		{
			switch (mode)
			{
				case CULL_BACK:
					return GL_BACK;
				case CULL_FRONT:
					return GL_FRONT;
				case CULL_FRONT_AND_BACK:
					return GL_FRONT_AND_BACK;
				default:
					return GL_FALSE;
			}
		}

		uint32_t getGLBlendFactor(BlendFactor blendFactor)
		{
			switch (blendFactor)
			{
				case pio::gfx::Bld_Factor_Zero:
					return GL_ZERO;
				case pio::gfx::Bld_Factor_One:
					return GL_ONE;
				case pio::gfx::Bld_Factor_SrcColor:
					return GL_SRC_COLOR;
				case pio::gfx::Bld_Factor_OneMinusSrcColor:
					return GL_ONE_MINUS_SRC_COLOR;
				case pio::gfx::Bld_Factor_SrcAlpha:
					return GL_SRC_ALPHA;
				case pio::gfx::Bld_Factor_OneMinusSrcAlpha:
					return GL_ONE_MINUS_SRC_ALPHA;
				case pio::gfx::Bld_Factor_DstAlpha:
					return GL_DST_ALPHA;
				case pio::gfx::Bld_Factor_OneMinusDstAlpha:
					return GL_ONE_MINUS_DST_ALPHA;
				case pio::gfx::Bld_Factor_DstColor:
					return GL_DST_COLOR;
				case pio::gfx::Bld_Factor_OneMinusDstColor:
					return GL_ONE_MINUS_DST_COLOR;
				case pio::gfx::Bld_Factor_SrcAlphaSaturate:
					return GL_SRC_ALPHA_SATURATE;
				case pio::gfx::Bld_Factor_ConstantColor:
					return GL_CONSTANT_COLOR;
				case pio::gfx::Bld_Factor_OneMinusConstantColor:
					return GL_ONE_MINUS_CONSTANT_COLOR;
				case pio::gfx::Bld_Factor_ConstantAlpha:
					return GL_CONSTANT_ALPHA;
				case pio::gfx::Bld_Factor_OneMinusConstantAlpha:
					return GL_ONE_MINUS_CONSTANT_ALPHA;
				default:
					return 0;
			}
		}

		State::State()
		{
		}

		State::~State() = default;

		bool State::init()
		{
			return true;
		}

		void State::shutdown()
		{
		}

		void State::setCullMode(const CullFace &mode)
		{
			if (mode.m_enable)
			{
				glEnable(GL_CULL_FACE);
				glFrontFace(getGLFaceDir(mode.m_faceDir));
				glCullFace(getGLCullMode(mode.m_mode));
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
		}

		void State::setBlendMode(const Blend &blend)
		{
			if (blend.m_enable)
			{
				glEnable(GL_BLEND);
				glBlendFunc(getGLBlendFactor(blend.m_src),
							getGLBlendFactor(blend.m_dest));
			}
			else
			{
				glDisable(GL_BLEND);
			}
		}

		void State::setDepthMode(const DepthTest &test)
		{
			if (test.m_enable)
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
		}
	}
}