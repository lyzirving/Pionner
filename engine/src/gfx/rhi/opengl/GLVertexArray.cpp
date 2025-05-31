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
	GLVertexArray::GLVertexArray(const Ref<RenderContext>& context) : VertexArray(context)
	{
	}

	bool GLVertexArray::Init()
	{
		if (!IsInit())
		{
			m_BindIndex = 0;
			glGenVertexArrays(1, &m_ID);
			if (GLHelper::CheckError("fail to generate vao[%u]", m_ID))
			{
				LOGD("create vertex array[%u]", m_ID);
			}
		}
		return IsInit();
	}

	void GLVertexArray::Destroy()
	{
		if (IsInit())
		{
			glDeleteVertexArrays(1, &m_ID);
			m_BindIndex = 0;
			m_ID = 0;
		}
	}

	void GLVertexArray::Bind()
	{
		Init();

		BindVertexBuffers();

		if (IsInit())
		{
			glBindVertexArray(m_ID);
			m_Bound = GLHelper::CheckError("err! fail to bind vertex array", m_ID);
		}
		else
		{
			LOGE("invalid state, vao[%u]", m_ID);
		}
	}

	void GLVertexArray::UnBind()
	{
		if (IsInit())
		{
			glBindVertexArray(0);
		}
		else
		{
			LOGE("invalid state, vao[%u]", m_ID);
		}
		m_Bound = false;
	}

	void GLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		if (!vertexBuffer || vertexBuffer->Layout().Elements().empty())
		{
			LOGE("invalid input vertex buffer");
			return;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void GLVertexArray::BindVertexBuffers()
	{
		if (IsInit() && m_BindIndex < m_VertexBuffers.size())
		{
			m_BindIndex = 0;
			uint32_t slot{ 0 };
			glBindVertexArray(m_ID);
			for (auto& buffer : m_VertexBuffers)
			{
				if (!buffer)
					continue;

				buffer->Bind();

				auto& layout = buffer->Layout();
				BindBufferLayout(layout, slot);

				m_BindIndex++;
			}
			GLHelper::CheckError("err when binding vertex buffer data");
			glBindVertexArray(0);
		}
	}

	void GLVertexArray::BindBufferLayout(const VertexBufferLayout& layout, uint32_t& slot)
	{
		const auto& elems = layout.Elements();

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
									  e.Normalized ? GL_TRUE : GL_FALSE, layout.Stride(), (const void*)e.Offset);
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
									   layout.Stride(), (const void*)e.Offset);
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
									   layout.Stride(), (const void*)e.Offset);
				slot++;
				break;
			}
			default:
				break;
			}
		}
	}
}
