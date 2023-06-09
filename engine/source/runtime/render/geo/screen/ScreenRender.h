#ifndef __RENDER_GEO_SCREEN_RENDER_H__
#define __RENDER_GEO_SCREEN_RENDER_H__

#include "render/geo/Geometry.h"
#include "render/shader/ShaderDef.h"

namespace pio
{
	class ScreenRender : public Geometry
	{
	public:
		ScreenRender(ShaderType shaderType, const std::string &name);
		virtual ~ScreenRender();

		virtual void initialize(RenderParam &param) override;

		inline void setTextureId(uint32_t id) { m_texId = id; }

	protected:
		ShaderType  m_shaderType;
		uint32_t    m_texId;
	};
}

#endif