#pragma once

#include "gfx/rhi/UniformBuffer.h"

namespace pio
{
	class UBufferProxy : public UniformBuffer
	{
		RTTR_ENABLE(UniformBuffer)
	public:
		UBufferProxy(const Ref<RenderContext>& context, const std::string& name);
		virtual ~UBufferProxy() = default;		
		virtual void Upload();

		// ----------------- Interface for RenderResource --------------------------
		virtual bool Init() override { return m_UBuffer->Init(); }
		virtual void Destroy() override { m_UBuffer->Destroy(); }
		virtual bool IsInit() const override { return m_UBuffer->IsInit(); }

		virtual void Bind() override { m_UBuffer->Bind(); }
		virtual void UnBind() override { m_UBuffer->UnBind(); }
		virtual bool IsBind() const override { return m_UBuffer->IsBind(); }
		virtual uint32_t Size() const override { return m_UBuffer->Size(); }
		virtual uint32_t Id() const override { return m_UBuffer->Id(); }
		// -------------------------------------------------------------------------

		virtual bool BindBlock(const Ref<RenderContext>& context, const Ref<Shader>& shader) override { return m_UBuffer->BindBlock(context, shader); }
		virtual void BindAt(int64_t offset, int64_t size) override { m_UBuffer->BindAt(offset, size); }
		virtual UBBinding Binding() const override { return m_UBuffer->Binding(); }

	protected:
		Ref<UniformBlock> m_UBlock;
		Ref<UniformBuffer> m_UBuffer;
	};
}