#include "GLIndexBuffer.h"

#include "GLHelper.h"
#include "GLHeader.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLIndexBuffer"

namespace pio
{
	GLIndexBuffer::GLIndexBuffer(Ref<RenderContext>& context, uint32_t size, uint32_t indexCount, IndexInternalFmt internalFmt, BufferUsage usage)
		: IndexBuffer(context), m_usage(usage), m_format(internalFmt), m_indexCount(indexCount)
	{
		m_size = size;
	}

	GLIndexBuffer::GLIndexBuffer(Ref<RenderContext>& context, const void* data, uint32_t size, uint32_t indexCount, IndexInternalFmt internalFmt, BufferUsage usage)
		: IndexBuffer(context), m_usage(usage), m_format(internalFmt), m_indexCount(indexCount)
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

	bool GLIndexBuffer::init()
	{
		if (!isInit())
		{
			glGenBuffers(1, &m_id);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GLHelper::BufferUsageToGLUsage(m_usage));
			if (GLHelper::CheckError("fail to make index buffer, vbo[%u]", m_id))
			{
				LOGD("create index buffer[%u]", m_id);
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
			glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
	}

	void GLIndexBuffer::bind()
	{
		init();

		if (isInit())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		}
		else
		{
			LOGE("invalid state, vbo[%u]", m_id);
		}
	}

	void GLIndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GLIndexBuffer::setIndice(const void* data, uint32_t size, uint32_t indiceNum, uint32_t offset)
	{
		if (!data || size == 0 || indiceNum == 0)
		{
			LOGE("data is invalid");
			return;
		}

		init();

		bool bSizeChange = (m_size != size);

		if (isInit())
		{			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
			if (bSizeChange)
			{
				m_size = size;
				m_indexCount = indiceNum;
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, data, GLHelper::BufferUsageToGLUsage(m_usage));
			}
			else if (m_size >= (size + offset))
			{
				m_indexCount = indiceNum;
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
			}
			else
			{
				LOGE("err! invalid input, buffer size[%u] < data size[%u] + offset[%u]", m_size, size, offset);
			}
			GLHelper::CheckError("fail to update GLIndexBuffer data");
		}
		else
		{
			LOGE("invalid state, vbo[%u]", m_id);
		}
	}
}