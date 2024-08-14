#ifndef __PIONNER_GFX_RENDERER_RENDER_COMMAND_H__
#define __PIONNER_GFX_RENDERER_RENDER_COMMAND_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class CRenderAPI;
	class UniformBuffer;

	class RenderCommand
	{
	public:
		static void UploadData(void *data, Ref<UniformBuffer> &buffer);

	private:
		static CRenderAPI *k_API;
	};
}

#endif