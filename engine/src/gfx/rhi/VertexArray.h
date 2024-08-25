#ifndef __PIONNER_GFX_RHI_VERTEX_ARRAY_H__
#define __PIONNER_GFX_RHI_VERTEX_ARRAY_H__

#include "VertexBuffer.h"

namespace pio
{
	class VertexArray : public RenderResource
	{
	public:
		VertexArray(Ref<RenderContext>& context) : RenderResource(context, RenderResourceType::VAO) {}
		virtual ~VertexArray() = default;

		virtual void addVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) = 0;
		virtual const std::vector<Ref<VertexBuffer>> &getVertexBuffers() const = 0;

	public:
		static Ref<VertexArray> Create(Ref<RenderContext>& context);
	};
}

#endif