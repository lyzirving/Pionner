#ifndef __PIONNER_GFX_RHI_RENDER_RESOURCE_H__
#define __PIONNER_GFX_RHI_RENDER_RESOURCE_H__

#include "RhiDef.h"

namespace pio
{
	class RenderContext;

	class RenderResource
	{
	public:
		RenderResource(Ref<RenderContext>& context) : m_context(context) {}
		virtual ~RenderResource()
		{			
			if (m_context)
			{
				//[TODO] relese render resource in render thread
			}
		}

		virtual bool init() = 0;
		virtual void destroy() = 0;
		virtual bool isInit() const = 0;

	protected:
		Ref<RenderContext> m_context;
	};
}

#endif