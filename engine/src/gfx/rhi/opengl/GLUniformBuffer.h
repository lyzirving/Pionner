#pragma once

#include "gfx/rhi/UniformBuffer.h"

namespace pio
{
	class GLUniformBuffer : public UniformBuffer
	{
		RTTR_ENABLE(UniformBuffer)
	public:
		GLUniformBuffer(const Ref<RenderContext>& context, uint32_t size, UBBinding binding, BufferUsage usage = BufferUsage::DynamicRead);
		virtual ~GLUniformBuffer() = default;

		virtual bool Init() override;
		virtual void Destroy() override;
		virtual bool IsInit() const override { return m_ID != 0; }

		virtual bool BindBlock(const Ref<RenderContext>& context, const Ref<Shader>& shader) override;
		virtual void Bind() override;
		virtual void UnBind() override;

		virtual uint32_t Size() const override { return m_Size; }
		virtual void SetData(const void *data, uint32_t size, uint32_t offset) override;

		virtual void BindAt(int64_t offset, int64_t size) override;
		virtual UBBinding Binding() const override { return m_Binding; };		

	private:
		BufferUsage m_Usage{ BufferUsage::Static };
		UBBinding m_Binding{ UBBinding_Num };
	};
}