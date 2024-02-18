#include "GLIndexBuffer.h"

#include "GLHelper.h"

#include "gfx/struct/Mesh.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLIndexBuffer"

namespace pio
{
	GLIndexBuffer::GLIndexBuffer(uint32_t size, uint32_t num) : IndexBuffer(), m_size(size), m_num(num)
	{
	}

	GLIndexBuffer::GLIndexBuffer(const void *data, uint32_t size, uint32_t num) : IndexBuffer(), m_size(size), m_num(num)
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

	GLIndexBuffer::~GLIndexBuffer()
	{
		destroy();
	}

	bool GLIndexBuffer::init()
	{
		if (!isInit())
		{
			glGenBuffers(1, &m_vbo);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_STATIC_DRAW);
			if (GLHelper::CheckError("fail to make index buffer, vbo[%u]", m_vbo))
			{
				LOGD("create index buffer[%u]", m_vbo);
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			if (m_data)
			{
				std::free(m_data);
				m_data = nullptr;
			}
		}
		return isInit();
	}

	void GLIndexBuffer::destroy()
	{
		if (isInit())
		{
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}
	}

	void GLIndexBuffer::bind()
	{
		init();

		if (isInit())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo);
		}
		else
		{
			LOGE("invalid state, vbo[%u]", m_vbo);
		}
	}

	void GLIndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GLIndexBuffer::setData(const void *data, uint32_t size, uint32_t indexCnt, uint32_t offset)
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

		m_num = indexCnt;

		if (isInit())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
			GLHelper::CheckError("fail to update GLIndexBuffer data");
		}
		else
		{
			LOGE("invalid state, vbo[%u]", m_vbo);
		}
	}
}