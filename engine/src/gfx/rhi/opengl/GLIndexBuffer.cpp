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
	GLIndexBuffer::GLIndexBuffer(const Ref<RenderContext>& context, uint32_t size, uint32_t IndexCount, IndexInternalFmt InternalFmt, BufferUsage usage)
		: IndexBuffer(context), m_Usage(usage), m_Format(InternalFmt), m_IndexCount(IndexCount)
	{
		m_Size = size;
	}

	GLIndexBuffer::GLIndexBuffer(const Ref<RenderContext>& context, const void* data, uint32_t size, uint32_t IndexCount, IndexInternalFmt InternalFmt, BufferUsage usage)
		: IndexBuffer(context), m_Usage(usage), m_Format(InternalFmt), m_IndexCount(IndexCount)
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

	bool GLIndexBuffer::Init()
	{
		if (!IsInit())
		{
			glGenBuffers(1, &m_ID);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Size, m_Data, GLHelper::BufferUsageToGLUsage(m_Usage));
			if (GLHelper::CheckError("fail to make index buffer, vbo[%u]", m_ID))
			{
				LOGD("create index buffer[%u]", m_ID);
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			if (m_Data)
			{
				std::free(m_Data);
				m_Data = nullptr;
			}
		}
		return IsInit();
	}

	void GLIndexBuffer::Destroy()
	{
		if (IsInit())
		{
			glDeleteBuffers(1, &m_ID);
			m_ID = 0;
		}
	}

	void GLIndexBuffer::Bind()
	{
		Init();

		if (IsInit())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			m_Bound = GLHelper::CheckError("fail to bind index buffer[%u]", m_ID);
		}
		else
		{
			LOGE("invalid state, vbo[%u]", m_ID);
		}
	}

	void GLIndexBuffer::UnBind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		m_Bound = false;
	}

	void GLIndexBuffer::SetIndice(const void* data, uint32_t size, uint32_t indiceNum, uint32_t offset)
	{
		if (!data || size == 0 || indiceNum == 0)
		{
			LOGE("data is invalid");
			return;
		}

		Init();

		bool bSizeChange = (m_Size != size);

		if (IsInit())
		{			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			if (bSizeChange)
			{
				m_Size = size;
				m_IndexCount = indiceNum;
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Size, data, GLHelper::BufferUsageToGLUsage(m_Usage));
			}
			else if (m_Size >= (size + offset))
			{
				m_IndexCount = indiceNum;
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
			}
			else
			{
				LOGE("err! invalid input, buffer size[%u] < data size[%u] + offset[%u]", m_Size, size, offset);
			}
			GLHelper::CheckError("fail to update GLIndexBuffer data");
		}
		else
		{
			LOGE("invalid state, vbo[%u]", m_ID);
		}
	}
}