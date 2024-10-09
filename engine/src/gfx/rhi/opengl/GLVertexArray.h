#ifndef __PIONNER_GFX_RHI_OPENGL_GL_VERTEX_ARRAY_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_VERTEX_ARRAY_H__

#include "gfx/rhi/VertexArray.h"

namespace pio
{
	class GLVertexArray : public VertexArray
	{
	public:
		GLVertexArray(Ref<RenderContext>& context);
		virtual ~GLVertexArray() = default;

		virtual bool init() override;
		virtual void destroy() override;
		virtual bool isInit() const override { return m_id != 0; }

		virtual void bind() override;
		virtual void unbind() override;

		virtual void addVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;
		virtual const std::vector<Ref<VertexBuffer>> &getVertexBuffers() const override { return m_vertexBuffers; }
		

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