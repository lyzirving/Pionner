#include "GLUniformBuffer.h"
#include "GLHelper.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLUniformBuffer"

namespace pio
{
	GLUniformBuffer::GLUniformBuffer(uint32_t size, uint32_t binding, BufferUsage usage)
		: UniformBuffer(), m_size(size), m_usage(usage), m_binding(binding)
	{
	}

	GLUniformBuffer::~GLUniformBuffer()
	{
		destroy();
	}

	bool GLUniformBuffer::init()
	{
		if (!isInit())
		{
			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_UNIFORM_BUFFER, m_vbo);
			glBufferData(GL_UNIFORM_BUFFER, m_size, nullptr, GLHelper::BufferUsageToGLUsage(m_usage));
			if (GLHelper::CheckError("fail to create uniform buffer[%u]", m_vbo))
			{
				LOGD("create uniform buffer[%u], binding[%u], byte used[%u]", m_vbo, m_binding, m_size);
			}
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			
		}
		return isInit();
	}

	void GLUniformBuffer::destroy()
	{
		if (isInit())
		{
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}
	}

	void GLUniformBuffer::bind()
	{
		init();

		if (isInit())
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_vbo);
			GLHelper::CheckError("fail to bind buffer[%u] to binding[%u]", m_vbo, m_binding);
		}
		else
		{
			LOGE("invalid state, vbo[%u], binding[%u]", m_vbo, m_binding);
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
			glBindBufferRange(GL_UNIFORM_BUFFER, m_binding, m_vbo, offset, size);
			GLHelper::CheckError("fail to bind buffer[%u] to binding[%u], offset[%ld], size[%ld]", 
								 m_vbo, m_binding, offset, size);
		}
		else
		{
			LOGE("invalid state, vbo[%u], binding[%u]", m_vbo, m_binding);
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
			glBindBuffer(GL_UNIFORM_BUFFER, m_vbo);
			glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
			GLHelper::CheckError("fail to update uniform buffer, offset[%u], size[%u]", offset, size);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
		else
		{
			LOGE("invalid state, vbo[%u], binding[%u]", m_vbo, m_binding);
		}
	}


	bool GLUniformBuffer::Binding(const Ref<Shader> &shader, const std::string &blockName, uint32_t bindingPt)
	{
		return false;
		//if (!shader || !shader->isInit())
		//{
		//	LOGE("invalid program");
		//	return false;
		//}

		//if (blockName.empty())
		//{
		//	LOGE("invalid block name");
		//	return false;
		//}

		//uint32_t program = shader->getProgram();

		//uint32_t index = glGetUniformBlockIndex(program, blockName.c_str());
		//if (index == GL_INVALID_INDEX)
		//{
		//	LOGE("fail to get block[%s]'s index in program[%u]", blockName.c_str(), program);
		//	GLHelper::CheckError("fail to get uniform block's index in program");
		//	return false;
		//}

		//GLint blockSize{ 0 };
		//glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
		////LOGD("block[%s] in program[%u]'s size is [%d]", blockName.c_str(), program, blockSize);

		//glUniformBlockBinding(program, index, bindingPt);
		//return GLHelper::CheckError("fail to bind program[%u]'s uniform block[%s][%u] to binding point[%u]",
		//					        program, blockName.c_str(), index, bindingPt);
	}
}