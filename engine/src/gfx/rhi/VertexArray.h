#pragma once

#include "VertexBuffer.h"

namespace pio
{
	class VertexArray : public RenderResource
	{
		RTTR_ENABLE(RenderResource)
	public:
		VertexArray(const Ref<RenderContext>& context) : RenderResource(context, RenderResourceType::VAO) {}
		virtual ~VertexArray() = default;

		virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) = 0;
		virtual const std::vector<Ref<VertexBuffer>> &GetVertexBuffers() const = 0;

	public:
		static Ref<VertexArray> Create(const Ref<RenderContext>& context);
	};
}