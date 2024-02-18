#ifndef __PIONNER_GFX_RHI_VERTEX_ARRAY_H__
#define __PIONNER_GFX_RHI_VERTEX_ARRAY_H__

#include "VertexBuffer.h"

namespace pio
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual bool init() = 0;
		virtual void destroy() = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void addVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) = 0;
		virtual const std::vector<Ref<VertexBuffer>> &getVertexBuffers() const = 0;

		virtual bool isInit() const = 0;

	public:
		static Ref<VertexArray> Create();
	};
}

#endif