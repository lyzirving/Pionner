#ifndef __PIONNER_PLATFORM_OPENGL_VERTEX_ARRAY_H__
#define __PIONNER_PLATFORM_OPENGL_VERTEX_ARRAY_H__

#include "gfx/rhi/VertexArray.h"

namespace pio
{
	class GLVertexArray : public VertexArray
	{
	public:
		GLVertexArray();
		virtual ~GLVertexArray();

		virtual bool init() override;
		virtual void destroy() override;

		virtual void bind() override;
		virtual void unbind() override;

		virtual void addVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>> &getVertexBuffers() const override { return m_vertexBuffers; }
		virtual bool isInit() const override { return m_vao != 0; }

	private:
		void bindVertexBuffers();
		void bindBufferLayout(const VertexBufferLayout &layout, uint32_t &slot);

	private:
		uint32_t m_vao{ 0 };
		uint32_t m_bindIndex{ 0 };
		std::vector<Ref<VertexBuffer>> m_vertexBuffers{};
	};
}

#endif