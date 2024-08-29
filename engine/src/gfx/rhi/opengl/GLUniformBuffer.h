#ifndef __PIONNER_GFX_RHI_OPENGL_GL_UNIFORM_BUFFER_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_UNIFORM_BUFFER_H__

#include "gfx/rhi/UniformBuffer.h"

namespace pio
{
	class GLUniformBuffer : public UniformBuffer
	{
	public:
		GLUniformBuffer(Ref<RenderContext>& context, uint32_t size, UBBinding binding, BufferUsage usage = BufferUsage::DynamicRead);
		virtual ~GLUniformBuffer() = default;

		virtual bool init() override;
		virtual void destroy() override;
		virtual bool isInit() const override { return m_id != 0; }

		virtual void bind() override;
		virtual void unbind() override;

		virtual uint32_t size() const override { return m_size; }
		virtual void setData(const void *data, uint32_t size, uint32_t offset) override;

		virtual void bind(int64_t offset, int64_t size) override;
		virtual UBBinding binding() const override { return m_binding; };		

	private:
		BufferUsage m_usage{ BufferUsage::Static };
		UBBinding m_binding{ UBBinding_Num };
	};
}

#endif