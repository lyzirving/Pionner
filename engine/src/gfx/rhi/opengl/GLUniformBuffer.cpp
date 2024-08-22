#include "GLUniformBuffer.h"
#include "GLHelper.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLUniformBuffer"

namespace pio
{
	GLUniformBuffer::GLUniformBuffer(Ref<RenderContext>& context, uint32_t size, UBBinding binding, BufferUsage usage)
		: UniformBuffer(context), m_size(size), m_usage(usage), m_binding(binding)
	{
	}

	GLUniformBuffer::~GLUniformBuffer()
	{
	}

	bool GLUniformBuffer::init()
	{
		if (!isInit())
		{
			glGenBuffers(1, &m_id);
			glBindBuffer(GL_UNIFORM_BUFFER, m_id);
			glBufferData(GL_UNIFORM_BUFFER, m_size, nullptr, GLHelper::BufferUsageToGLUsage(m_usage));
			if (GLHelper::CheckError("fail to create uniform buffer[%u]", m_id))
			{
				LOGD("create uniform buffer[%u], binding[%u], byte used[%u]", m_id, m_binding, m_size);
			}
			glBindBuffer(GL_UNIFORM_BUFFER, 0);			
		}
		return isInit();
	}

	void GLUniformBuffer::destroy()
	{
		if (isInit())
		{
			glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
	}

	void GLUniformBuffer::bind()
	{
		init();

		if (isInit())
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_id);
			GLHelper::CheckError("fail to bind buffer[%u] to binding[%u]", m_id, m_binding);
		}
		else
		{
			LOGE("invalid state, vbo[%u], binding[%u]", m_id, m_binding);
		}
	}

	void GLUniformBuffer::bind(int64_t offset, int64_t size)
	{
		init();

		if (m_size < (offset + size))
		{
			LOGE("invalid bind, offset[%u] + size[%u] > raw size[%u], binding[%u]", 
				 offset, size, m_size, m_binding);
			return;
		}

		if (isInit())
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, m_binding, m_id, offset, size);
			GLHelper::CheckError("fail to bind buffer[%u] to binding[%u], offset[%ld], size[%ld]", 
								 m_id, m_binding, offset, size);
		}
		else
		{
			LOGE("invalid state, vbo[%u], binding[%u]", m_id, m_binding);
		}
	}

	void GLUniformBuffer::unbind()
	{
		if (isInit())
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, 0);
		}
	}

	void GLUniformBuffer::setData(const void *data, uint32_t size, uint32_t offset)
	{
		if (!data || size == 0)
		{
			LOGE("data is invalid");
			return;
		}

		if (m_size < (size + offset))
		{
			LOGE("invalid size, byte size[%u] < [%] + [%]", m_size, size, offset);
			return;
		}

		init();

		if (isInit())
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_id);
			glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
			GLHelper::CheckError("fail to update uniform buffer, offset[%u], size[%u]", offset, size);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
		else
		{
			LOGE("invalid state, vbo[%u], binding[%u]", m_id, m_binding);
		}
	}
}