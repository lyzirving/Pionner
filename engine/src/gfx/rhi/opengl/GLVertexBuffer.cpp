#include "GLVertexBuffer.h"

#include "GLHelper.h"
#include "GLHeader.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLVertexBuffer"

namespace pio
{
	GLVertexBuffer::GLVertexBuffer(const Ref<RenderContext>& context, uint32_t size, BufferUsage usage)
		: VertexBuffer(context), m_Usage(usage)
	{
		m_Size = size;
	}

	GLVertexBuffer::GLVertexBuffer(const Ref<RenderContext>& context, const void *data, uint32_t size, BufferUsage usage)
		: VertexBuffer(context), m_Usage(usage)
	{
		m_Size = size;

		if (data && size != 0)
		{
			m_Data = std::malloc(size);
			if (m_Data)
			{
				std::memcpy(m_Data, data, size);
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

	bool GLVertexBuffer::Init()
	{
		if (!IsInit())
		{
			glGenBuffers(1, &m_ID);

			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, m_Size, m_Data, GLHelper::BufferUsageToGLUsage(m_Usage));
			if (GLHelper::CheckError("fail to make vertex buffer, vbo[%u]", m_ID))
			{
				LOGD("create vertex buffer[%u]", m_ID);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			if (m_Data)
			{
				std::free(m_Data);
				m_Data = nullptr;
			}
		}
		return IsInit();
	}

	void GLVertexBuffer::Destroy()
	{
		if (IsInit())
		{
			glDeleteBuffers(1, &m_ID);
			m_ID = 0;
		}
	}

	void GLVertexBuffer::Bind() 
	{
		Init();

		if (IsInit())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			m_Bound = GLHelper::CheckError("err! fail to bind vertex buffer", m_ID);
		}
		else
		{
			LOGE("invalid state, vbo[%u]", m_ID);
		}
	}

	void GLVertexBuffer::UnBind() 
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_Bound = false;
	}

	void GLVertexBuffer::SetData(const void *data, uint32_t size, uint32_t offset)
	{
		if (!data || size == 0)
		{
			LOGE("data is invalid");
			return;
		}		

		Init();

		if (IsInit())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			bool bSizeChange = (m_Size != size);
			if (bSizeChange)
			{
				m_Size = size;
				glBufferData(GL_ARRAY_BUFFER, m_Size, data, GLHelper::BufferUsageToGLUsage(m_Usage));
			}
			else if (m_Size >= (size + offset))
			{
				glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
			}
			else
			{
				LOGE("err! invalid input, buffer size[%u] < data size[%u] + offset[%u]", m_Size, size, offset);
			}
			GLHelper::CheckError("GLVertexBuffer: fail to update data");
		}
		else
		{
			LOGE("invalid state, vbo[%u]", m_ID);
		}
	}
}