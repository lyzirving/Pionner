#ifndef __PIONNER_GFX_RHI_RENDER_RESOURCE_H__
#define __PIONNER_GFX_RHI_RENDER_RESOURCE_H__

#include "RhiDef.h"

namespace pio
{
	class RenderContext;
	
	struct ResourceGCDelegate
	{
		RenderResourceType Type{ RenderResourceType::Num };
		uint32_t Id{ 0 };

		ResourceGCDelegate() {}
		ResourceGCDelegate(RenderResourceType t, uint32_t id) : Type(t), Id(id) {}
	};

	class RenderResource
	{
	public:
		RenderResource(Ref<RenderContext> &context, RenderResourceType type);
		virtual ~RenderResource();

		virtual bool init() = 0;
		virtual void destroy() = 0;
		virtual bool isInit() const = 0;

		RenderResourceType type() const { return m_type; }
		uint32_t id() const { return m_id; }

	protected:
		Ref<RenderContext> m_context;
		RenderResourceType m_type;
		uint32_t m_id{ 0 };
	};
}

#endif