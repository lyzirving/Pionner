#include <iostream>
#include <fstream>
#include <sstream>

#include "GLShader.h"
#include "GLHelper.h"
#include "GLHeader.h"

#include "gfx/rhi/UniformData.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLShader"

namespace pio
{
	GLShader::GLShader(Ref<RenderContext>& context, const std::string& name) : Shader(context, name)
	{
	}

	void GLShader::setFloat(const std::string& name, float val)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniform1f(ind, val);
			GLHelper::CheckError("setFloat err, program[%u], name[%s], val[%f]", m_id, name.c_str(), val);
			//LOGD("name[%s], val[%f]", name.c_str(), val);
		}
	}

	void GLShader::setInt(const std::string& name, int32_t val)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniform1i(ind, val);
			GLHelper::CheckError("setInt err, program[%u], name[%s], val[%d]", m_id, name.c_str(), val);
			//LOGD("name[%s], val[%d]", name.c_str(), val);
		}
	}

	void GLShader::setUInt(const std::string& name, uint32_t val)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniform1ui(ind, val);
			GLHelper::CheckError("setUInt err, program[%u], name[%s], val[%u]", m_id, name.c_str(), val);
			//LOGD("name[%s], val[%u]", name.c_str(), val);
		}
	}

	void GLShader::setBool(const std::string& name, bool val)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniform1i(ind, val ? 1 : 0);
			GLHelper::CheckError("setBool err, program[%u], name[%s], val[%s]", m_id, name.c_str(), val ? "true" : "false");
			//LOGD("name[%s], val[%s]", name.c_str(), val ? "true" : "false");
		}
	}

	void GLShader::setMat2(const std::string& name, const glm::mat2& mat)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniformMatrix2fv(ind, 1, GL_FALSE, glm::value_ptr(mat));
			GLHelper::CheckError("setMat2 err, program[%u], name[%s]", m_id, name.c_str());
		}
	}

	void GLShader::setMat3(const std::string& name, const glm::mat3& mat)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniformMatrix3fv(ind, 1, GL_FALSE, glm::value_ptr(mat));
			GLHelper::CheckError("setMat3 err, program[%u], name[%s]", m_id, name.c_str());
			/*LOGD("name[%s], val[%f, %f, %f\n%f, %f, %f\n%f, %f, %f]", name.c_str(),
				 mat[0].x, mat[0].y, mat[0].z,
				 mat[1].x, mat[1].y, mat[1].z,
				 mat[2].x, mat[2].y, mat[2].z);*/
		}
	}

	void GLShader::setMat4(const std::string& name, const glm::mat4& mat)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniformMatrix4fv(ind, 1, GL_FALSE, glm::value_ptr(mat));
			GLHelper::CheckError("setMat4 err, program[%u], name[%s]", m_id, name.c_str());
			/*LOGD("name[%s], val[%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f]", name.c_str(),
				 mat[0].x, mat[0].y, mat[0].z, mat[0].w,
				 mat[1].x, mat[1].y, mat[1].z, mat[1].w,
				 mat[2].x, mat[2].y, mat[2].z, mat[2].w,
				 mat[3].x, mat[3].y, mat[3].z, mat[3].w);*/
		}
	}

	void GLShader::setVec2(const std::string& name, const glm::vec2& vec2)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniform2f(ind, vec2.x, vec2.y);
			GLHelper::CheckError("setVec2 err, program[%u], name[%s], val[%f, %f]", m_id, name.c_str(), vec2.x, vec2.y);
			//LOGD("name[%s], val[%f, %f]", name.c_str(), vec2.x, vec2.y);
		}
	}

	void GLShader::setVec3(const std::string& name, const glm::vec3& vec3)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniform3f(ind, vec3.x, vec3.y, vec3.z);
			GLHelper::CheckError("setVec3 err, program[%u], name[%s], val[%f, %f, %f]", m_id, name.c_str(), vec3.x, vec3.y, vec3.z);
			//LOGD("name[%s], val[%f, %f, %f]", name.c_str(), vec3.x, vec3.y, vec3.z);
		}
	}

	void GLShader::setVec4(const std::string& name, const glm::vec4& vec4)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniform4f(ind, vec4.x, vec4.y, vec4.z, vec4.w);
			GLHelper::CheckError("setVec4 err, program[%u], name[%s], val[%f, %f, %f£¬ %f]", m_id, name.c_str(), vec4.x, vec4.y, vec4.z, vec4.w);
			//LOGD("name[%s], val[%f, %f, %f, %f]", name.c_str(), vec4.x, vec4.y, vec4.z, vec4.w);
		}
	}

	void GLShader::setIVec2(const std::string& name, const glm::ivec2& vec2)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniform2i(ind, vec2.x, vec2.y);
			GLHelper::CheckError("setIVec2 err, program[%u], name[%s], val[%d, %d]", m_id, name.c_str(), vec2.x, vec2.y);
			//LOGD("name[%s], val[%d, %d]", name.c_str(), vec2.x, vec2.y);
		}
	}

	void GLShader::setIVec3(const std::string& name, const glm::ivec3& vec3)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniform3i(ind, vec3.x, vec3.y, vec3.z);
			GLHelper::CheckError("setIVec3 err, program[%u], name[%s], val[%d, %d, %d]", m_id, name.c_str(), vec3.x, vec3.y, vec3.z);
			//LOGD("name[%s], val[%d, %d, %d]", name.c_str(), vec3.x, vec3.y, vec3.z);
		}
	}

	void GLShader::setIVec4(const std::string& name, const glm::ivec4& vec4)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniform4i(ind, vec4.x, vec4.y, vec4.z, vec4.w);
			GLHelper::CheckError("setIVec4 err, program[%u], name[%s], val[%d, %d, %d, %d]", m_id, name.c_str(), vec4.x, vec4.y, vec4.z, vec4.w);
			//LOGD("name[%s], val[%d, %d, %d, %d]", name.c_str(), vec4.x, vec4.y, vec4.z, vec4.w);
		}
	}

	void GLShader::setTextureSampler(const std::string& name, TextureSampler slot)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_id, name.c_str());
			glUniform1i(ind, (int32_t)slot);
			m_occupiedSlots.set((int32_t)slot);
			GLHelper::CheckError("setTextureSampler err, program[%u], name[%s], slot[%d]",
								 m_id, name.c_str(), (int32_t)slot);
			//LOGD("name[%s], slot[%d]", name.c_str(), (int32_t)slot);
		}
	}

	void GLShader::freeSamplerUnit()
	{
		m_occupiedSlots.reset();
	}

	bool GLShader::init()
	{
		if (isInit())
			return true;

		if (m_vert.empty() || m_frag.empty())
		{
			LOGE("source is invalid");
			return false;
		}

		if (!m_geo.empty())
		{
			m_id = GLHelper::CreateShaderProgram(m_vert.c_str(), m_frag.c_str(), m_geo.c_str());
		}
		else
		{
			m_id = GLHelper::CreateShaderProgram(m_vert.c_str(), m_frag.c_str());
		}

		bool success = m_id > 0;
		if (success)
		{
			LOGD("succeed to init shader[%s][%u]", m_name.c_str(), m_id);
		}
		return success;
	}

	void GLShader::destroy()
	{
		if (isInit())
		{
			glDeleteProgram(m_id);
			m_id = 0;
			m_occupiedSlots.reset();
		}
	}

	void GLShader::bind()
	{
		init();

		if (isInit())
		{
			glUseProgram(m_id);
			m_occupiedSlots.reset();
			GLHelper::CheckError("err! program[%u]", m_id);
		}
	}

	void GLShader::unbind()
	{
		glUseProgram(0);
	}

	bool GLShader::getSampler(TextureSampler& out) const
	{
		bool found{ false };
		if (m_occupiedSlots.any())
		{
			for (uint8_t i = 0; i < (uint8_t)TextureSampler::MaxSlotNum; i++)
			{
				if (!m_occupiedSlots.test(i))
				{
					out = static_cast<TextureSampler>(i);
					found = true;
					break;
				}
			}
		}
		else
		{
			out = TextureSampler::Slot0;
			found = true;
		}

		if (!found)
			LOGE("no valid sampler for shader[%s]", m_name.c_str());

		return found;
	}
}