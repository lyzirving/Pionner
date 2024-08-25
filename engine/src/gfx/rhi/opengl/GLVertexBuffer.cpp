#include "GLVertexBuffer.h"

#include "GLHelper.h"
#include "GLHeader.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLVertexBuffer"

namespace pio
{
	GLVertexBuffer::GLVertexBuffer(Ref<RenderContext>& context, uint32_t size, BufferUsage usage)
		: VertexBuffer(context), m_usage(usage)
	{
		m_size = size;
	}

	GLVertexBuffer::GLVertexBuffer(Ref<RenderContext>& context, const void *data, uint32_t size, BufferUsage usage)
		: VertexBuffer(context), m_usage(usage)
	{
		m_size = size;

		if (data && size != 0)
		{
			m_data = std::malloc(size);
			if (m_data)
			{
				std::memcpy(m_data, data, size);
			}
			else
			{
				LOGE("fail to allocate [%u] memory", size);
			}
		}
		else
		{
			LOGE("invalid input data, empty[%s], size[%u]", data == nullptr ? "true" : "false", size);
		}
	}

	bool GLVertexBuffer::init()
	{
		if (!isInit())
		{
			glGenBuffers(1, &m_id);

			glBindBuffer(GL_ARRAY_BUFFER, m_id);
			glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GLHelper::BufferUsageToGLUsage(m_usage));
			if (GLHelper::CheckError("fail to make vertex buffer, vbo[%u]", m_id))
			{
				LOGD("create vertex buffer[%u]", m_id);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			if (m_data)
			{
				std::free(m_data);
				m_data = nullptr;
			}
		}
		return isInit();
	}

	void GLVertexBuffer::destroy()
	{
		if (isInit())
		{
			glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
	}

	void GLVertexBuffer::bind() 
	{
		init();

		if (isInit())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
		}
		else
		{
			LOGE("invalid state, vbo[%u]", m_id);
		}
	}

	void GLVertexBuffer::unbind() 
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLVertexBuffer::setData(const void *data, uint32_t size, uint32_t offset)
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
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
			GLHelper::CheckError("GLVertexBuffer: fail to update data");
		}
		else
		{
			LOGE("invalid state, vbo[%u]", m_id);
		}
	}
}