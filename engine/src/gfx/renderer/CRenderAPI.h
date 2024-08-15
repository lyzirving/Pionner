#ifndef __PIONNER_GFX_RENDERER_CRENDER_API_H__
#define __PIONNER_GFX_RENDERER_CRENDER_API_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class CRenderAPI
	{
	public:
		virtual ~CRenderAPI() = default;

		virtual bool init() = 0;
		virtual void shutdown() = 0;
		virtual CRenderApiType type() const = 0;

	public:
		static Ref<CRenderAPI> Create(CRenderApiType type);
	};
}

#endif