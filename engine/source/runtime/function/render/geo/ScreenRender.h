#ifndef __RENDER_GEO_SCREEN_RENDER_H__
#define __RENDER_GEO_SCREEN_RENDER_H__

#include "Geometry.h"

namespace Pionner
{
	class ScreenRender : Geometry
	{
	public:
		ScreenRender();
		virtual ~ScreenRender();

		virtual void draw(RenderParam &param) override;
		virtual void initialize(RenderParam &param) override;

		inline void setTextureId(uint32_t id) { m_texId = id; }

	protected:
		virtual bool dealShader(RenderParam &param, std::shared_ptr<Shader> &shader) override;

	private:
		uint32_t m_texId;
	};
}

#endif