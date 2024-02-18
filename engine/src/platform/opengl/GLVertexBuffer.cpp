#include "GLVertexBuffer.h"
#include "GLHelper.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLVertexBuffer"

namespace pio
{
	GLVertexBuffer::GLVertexBuffer(uint32_t size, BufferUsage usage) 
		: VertexBuffer(), m_size(size), m_usage(usage)
	{
	}

	GLVertexBuffer::GLVertexBuffer(const void *data, uint32_t size, BufferUsage usage)
		: VertexBuffer(), m_size(size), m_usage(usage)
	{
		if (!data || size == 0)
		{
			LOGE("invalid input data, empty[%s], size[%u]", data == nullptr ? "true" : "false", size);
			return;
		}
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

	GLVertexBuffer::~GLVertexBuffer()
	{
		destroy();
	}

	bool GLVertexBuffer::init()
	{
		if (!isInit())
		{
			glGenBuffers(1, &m_vbo);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GLHelper::BufferUsageToGLUsage(m_usage));
			if (GLHelper::CheckError("fail to make vertex buffer, vbo[%u]", m_vbo))
			{
				LOGD("create vertex buffer[%u]", m_vbo);
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
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}
	}

	void GLVertexBuffer::bind() 
	{
		init();

		if (isInit())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		}
		else
		{
			LOGE("invalid state, vbo[%u]", m_vbo);
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
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		}
		else
		{
			LOGE("invalid state, vbo[%u]", m_vbo);
		}
	}
}