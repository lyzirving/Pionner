#ifndef __PIONNER_GFX_DATA_RENDERING_DATA_H__
#define __PIONNER_GFX_DATA_RENDERING_DATA_H__

#include "gfx/rhi/UniformBuffer.h"

namespace pio
{
	class RenderContext;

	class RenderingData
	{
	public:
		RenderingData() {}
		~RenderingData() = default;

		virtual void setUpData(Ref<RenderContext> &context) = 0;
		virtual Ref<UniformBuffer> &uniformBuffer() = 0;
	};
}

#endif