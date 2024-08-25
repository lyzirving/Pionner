#include "GLVertexArray.h"

#include "GLHelper.h"
#include "GLHeader.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLVertexArray"

namespace pio
{
	GLVertexArray::GLVertexArray(Ref<RenderContext>& context) : VertexArray(context)
	{
	}

	bool GLVertexArray::init()
	{
		if (!isInit())
		{
			m_bindIndex = 0;
			glGenVertexArrays(1, &m_id);
			if (GLHelper::CheckError("fail to generate vao[%u]", m_id))
			{
				LOGD("create vertex array[%u]", m_id);
			}
		}
		return isInit();
	}

	void GLVertexArray::destroy()
	{
		if (isInit())
		{
			glDeleteVertexArrays(1, &m_id);
			m_bindIndex = 0;
			m_id = 0;
		}
	}

	void GLVertexArray::bind()
	{
		init();

		bindVertexBuffers();

		if (isInit())
		{
			glBindVertexArray(m_id);
		}
		else
		{
			LOGE("invalid state, vao[%u]", m_id);
		}
	}

	void GLVertexArray::unbind()
	{
		if (isInit())
		{
			glBindVertexArray(0);
		}
		else
		{
			LOGE("invalid state, vao[%u]", m_id);
		}
	}

	void GLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		if (!vertexBuffer || vertexBuffer->layout().elements().empty())
		{
			LOGE("invalid input vertex buffer");
			return;
		}
		m_vertexBuffers.push_back(vertexBuffer);
	}

	void GLVertexArray::bindVertexBuffers()
	{
		if (isInit() && m_bindIndex < m_vertexBuffers.size())
		{
			m_bindIndex = 0;
			uint32_t slot{ 0 };
			glBindVertexArray(m_id);
			for (auto& buffer : m_vertexBuffers)
			{
				if (!buffer)
					continue;

				buffer->bind();

				auto& layout = buffer->layout();
				bindBufferLayout(layout, slot);

				m_bindIndex++;
			}
			GLHelper::CheckError("err when binding vertex buffer data");
			glBindVertexArray(0);
		}
	}

	void GLVertexArray::bindBufferLayout(const VertexBufferLayout& layout, uint32_t& slot)
	{
		const auto& elems = layout.elements();

		for (const auto& e : elems)
		{
			switch (e.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(slot);
				glVertexAttribPointer(slot, e.Comp, GLHelper::ShaderDataTypeToGLBaseType(e.Type),
									  e.Normalized ? GL_TRUE : GL_FALSE, layout.stride(), (const void*)e.Offset);
				slot++;
				break;
			}
			case ShaderDataType::Byte:
			case ShaderDataType::Byte2:
			case ShaderDataType::Byte3:
			case ShaderDataType::Byte4:
			case ShaderDataType::UByte:
			case ShaderDataType::UByte2:
			case ShaderDataType::UByte3:
			case ShaderDataType::UByte4:
			case ShaderDataType::Short:
			case ShaderDataType::Short2:
			case ShaderDataType::Short3:
			case ShaderDataType::Short4:
			case ShaderDataType::UShort:
			case ShaderDataType::UShort2:
			case ShaderDataType::UShort3:
			case ShaderDataType::UShort4:
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::UInt:
			case ShaderDataType::UInt2:
			case ShaderDataType::UInt3:
			case ShaderDataType::UInt4:
			{
				glEnableVertexAttribArray(slot);
				glVertexAttribIPointer(slot, e.Comp, GLHelper::ShaderDataTypeToGLBaseType(e.Type),
									   layout.stride(), (const void*)e.Offset);
				slot++;
				break;
			}
			case ShaderDataType::Double:
			case ShaderDataType::Double2:
			case ShaderDataType::Double3:
			case ShaderDataType::Double4:
			{
				glEnableVertexAttribArray(slot);
				glVertexAttribLPointer(slot, e.Comp, GLHelper::ShaderDataTypeToGLBaseType(e.Type),
									   layout.stride(), (const void*)e.Offset);
				slot++;
				break;
			}
			default:
				break;
			}
		}
	}
}
