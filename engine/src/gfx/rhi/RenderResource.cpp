#include "RenderResource.h"

#include "gfx/renderer/RenderContext.h"

namespace pio
{
	RenderResource::RenderResource(Ref<RenderContext> &context, RenderResourceType type) : m_context(context), m_type(type) 
	{
	}

	RenderResource::~RenderResource()
	{
		if(m_context)
		{
			ResourceGCDelegate resource(m_type, m_id);
			m_context->recyleResource(std::move(resource));
		}
	}
}