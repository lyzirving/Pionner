#include "UBufferProxy.h"

#include "RenderContext.h"

namespace pio
{
	UBufferProxy::UBufferProxy(const Ref<RenderContext>& context, const std::string& name) : UniformBuffer(context, name, RenderResourceType::UBOProxy)
	{
	}

	void UBufferProxy::Upload()
	{
		if(m_UBlock && m_UBuffer && m_UBlock->AnyChange())
		{
			m_Context.lock()->UploadData(m_UBlock->GetBuffer()->As<void*>(), m_UBlock->GetByteUsed(), m_UBuffer);
			m_UBlock->Invalidate(false);
		}
	}
}