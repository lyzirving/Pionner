#include "GLUniformBuffer.h"

#include "GLHelper.h"
#include "GLHeader.h"

#include "gfx/rhi/Shader.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLUniformBuffer"

namespace pio
{
	GLUniformBuffer::GLUniformBuffer(const Ref<RenderContext>& context, uint32_t size, UBBinding binding, BufferUsage usage)
		: UniformBuffer(context, GetBindingName(binding)), m_Usage(usage), m_Binding(binding)
	{
		m_Size = size;
	}

	bool GLUniformBuffer::Init()
	{
		if (!IsInit())
		{
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
			glBufferData(GL_UNIFORM_BUFFER, m_Size, nullptr, GLHelper::BufferUsageToGLUsage(m_Usage));
			if (GLHelper::CheckError("fail to create uniform buffer[%u]", m_ID))
			{
				LOGD("create uniform buffer[%u], binding[%u], byte used[%u]", m_ID, m_Binding, m_Size);
			}
			glBindBuffer(GL_UNIFORM_BUFFER, 0);			
		}
		return IsInit();
	}

	void GLUniformBuffer::Destroy()
	{
		if (IsInit())
		{
			glDeleteBuffers(1, &m_ID);
			m_ID = 0;
		}
	}

	bool GLUniformBuffer::BindBlock(const Ref<RenderContext>& context, const Ref<Shader>& shader)
	{
		if(!context)
		{
			LOGE("err! context is invalid for buffer[%s]", m_Name.c_str());
			return false;
		}

		if(!shader || !shader->IsBind())
		{
			LOGE("err! shader is invalid for buffer[%s]", m_Name.c_str());
			return false;
		}

		if(!context->BindUnimBlock(shader, Self<UniformBuffer>(), m_Name))
		{
			LOGE("err! u-buffer[%s] fail to bind block", m_Name.c_str());
			return false;
		}
		Bind();
		return IsBind();
	}

	void GLUniformBuffer::Bind()
	{
		Init();

		if (IsInit())
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, m_ID);
			m_Bound = GLHelper::CheckError("fail to bind buffer[%u] to binding[%u]", m_ID, m_Binding);
		}
		else
		{
			LOGE("invalid state, vbo[%u], binding[%u]", m_ID, m_Binding);
		}
	}

	void GLUniformBuffer::BindAt(int64_t offset, int64_t size)
	{
		Init();

		if (m_Size < (offset + size))
		{
			LOGE("invalid bind, offset[%u] + size[%u] > raw size[%u], binding[%u]", 
				 offset, size, m_Size, m_Binding);
			return;
		}

		if (IsInit())
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, m_Binding, m_ID, offset, size);
			m_Bound = GLHelper::CheckError("fail to bind buffer[%u] to binding[%u], offset[%ld], size[%ld]",
								            m_ID, m_Binding, offset, size);
		}
		else
		{
			LOGE("invalid state, vbo[%u], binding[%u]", m_ID, m_Binding);
		}
	}

	void GLUniformBuffer::UnBind()
	{
		if (IsInit())
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, 0);
		}
		m_Bound = false;
	}

	void GLUniformBuffer::SetData(const void *data, uint32_t size, uint32_t offset)
	{
		if (!data || size == 0)
		{
			LOGE("data is invalid");
			return;
		}

		if (m_Size < (size + offset))
		{
			LOGE("invalid size, byte size[%u] < [%] + [%]", m_Size, size, offset);
			return;
		}

		Init();

		if (IsInit())
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
			glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
			GLHelper::CheckError("fail to update uniform buffer, offset[%u], size[%u]", offset, size);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
		else
		{
			LOGE("invalid state, vbo[%u], binding[%u]", m_ID, m_Binding);
		}
	}
}