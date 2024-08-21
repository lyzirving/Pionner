#ifndef __PIONNER_GFX_DATA_DATA_WRAP_H__
#define __PIONNER_GFX_DATA_DATA_WRAP_H__

#include "gfx/rhi/UniformBuffer.h"

namespace pio
{
	class RenderContext;

	class DataWrap
	{
	public:
		DataWrap() {}
		~DataWrap() = default;

		virtual void setUp(Ref<RenderContext> &context) = 0;
		virtual Ref<UniformBuffer> &uniformBuffer() = 0;
	};
}

#endif