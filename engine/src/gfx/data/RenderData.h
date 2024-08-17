#ifndef __PIONNER_GFX_DATA_RENDER_DATA_H__
#define __PIONNER_GFX_DATA_RENDER_DATA_H__

#include "gfx/GfxDef.h"
#include "gfx/rhi/UniformBuffer.h"

namespace pio
{
	class RenderData
	{
	public:
		RenderData() {}
		RenderData(const std::string &name) : m_name(name) {}
		virtual ~RenderData() = default;

		const std::string &name() const { return m_name; }

	protected:
		virtual void initData() {}

	protected:
		std::string m_name;
		Ref<UniformBuffer> m_uniformBuffer;
	};
}

#endif