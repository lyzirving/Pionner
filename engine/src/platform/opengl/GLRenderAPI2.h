#ifndef __PIONNER_PLATFORM_OPENGL_GL_RENDER_API_2_H__
#define __PIONNER_PLATFORM_OPENGL_GL_RENDER_API_2_H__

#include "gfx/renderer/CRenderAPI.h"

namespace pio
{
	class GLRenderAPI2 : public CRenderAPI
	{
	public:
		GLRenderAPI2(CRenderApiType type);
		~GLRenderAPI2() = default;

		virtual bool init() override;
		virtual void shutdown() override;

		virtual CRenderApiType type() const override { return m_type; }

	private:
		const CRenderApiType m_type;
	};
}

#endif