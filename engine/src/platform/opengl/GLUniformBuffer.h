#ifndef __PIONNER_PLATFORM_GL_UNIFORM_BUFFER_H__
#define __PIONNER_PLATFORM_GL_UNIFORM_BUFFER_H__

#include "gfx/rhi/UniformBuffer.h"

namespace pio
{
	class GLUniformBuffer : public UniformBuffer
	{
	public:
		GLUniformBuffer(uint32_t size, uint32_t binding, BufferUsage usage = BufferUsage::DynamicRead);
		virtual ~GLUniformBuffer();

		virtual bool init() override;
		virtual void destroy() override;

		virtual void bind() override;
		virtual void bind(int64_t offset, int64_t size) override;
		virtual void unbind() override;

		virtual void setData(const void *data, uint32_t size, uint32_t offset) override;

		virtual bool isInit() const override { return m_vbo != 0; }
		virtual uint32_t getBinding() const override { return m_binding; };
		virtual uint32_t getSize() const override { return m_size; }

	public:
		static bool Binding(const Ref<Shader> &shader, const std::string &blockName, uint32_t bindingPt);

	private:
		uint32_t m_vbo{ 0 };
		uint32_t m_size{ 0 };
		BufferUsage m_usage{ BufferUsage::None };
		uint32_t m_binding{ 0 };
	};
}

#endif