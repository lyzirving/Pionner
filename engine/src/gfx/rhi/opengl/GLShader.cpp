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
	GLShader::GLShader(const Ref<RenderContext>& context, const std::string& name,
					   const std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource) 
		: Shader(context, name), m_StageSource(stageSource)
	{
	}

	void GLShader::SetFloat(const std::string& name, float val)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniform1f(ind, val);
			GLHelper::CheckError("SetFloat err, program[%u], name[%s], val[%f]", m_ID, name.c_str(), val);
			//LOGD("name[%s], val[%f]", name.c_str(), val);
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetInt(const std::string& name, int32_t val)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniform1i(ind, val);
			GLHelper::CheckError("SetInt err, program[%u], name[%s], val[%d]", m_ID, name.c_str(), val);
			//LOGD("name[%s], val[%d]", name.c_str(), val);
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetUInt(const std::string& name, uint32_t val)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniform1ui(ind, val);
			GLHelper::CheckError("SetUInt err, program[%u], name[%s], val[%u]", m_ID, name.c_str(), val);
			//LOGD("name[%s], val[%u]", name.c_str(), val);
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetBool(const std::string& name, bool val)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniform1i(ind, val ? 1 : 0);
			GLHelper::CheckError("SetBool err, program[%u], name[%s], val[%s]", m_ID, name.c_str(), val ? "true" : "false");
			//LOGD("name[%s], val[%s]", name.c_str(), val ? "true" : "false");
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetMat2(const std::string& name, const glm::mat2& mat)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniformMatrix2fv(ind, 1, GL_FALSE, glm::value_ptr(mat));
			GLHelper::CheckError("SetMat2 err, program[%u], name[%s]", m_ID, name.c_str());
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetMat3(const std::string& name, const glm::mat3& mat)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniformMatrix3fv(ind, 1, GL_FALSE, glm::value_ptr(mat));
			GLHelper::CheckError("SetMat3 err, program[%u], name[%s]", m_ID, name.c_str());
			/*LOGD("name[%s], val[%f, %f, %f\n%f, %f, %f\n%f, %f, %f]", name.c_str(),
				 mat[0].x, mat[0].y, mat[0].z,
				 mat[1].x, mat[1].y, mat[1].z,
				 mat[2].x, mat[2].y, mat[2].z);*/
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetMat4(const std::string& name, const glm::mat4& mat)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniformMatrix4fv(ind, 1, GL_FALSE, glm::value_ptr(mat));
			GLHelper::CheckError("SetMat4 err, program[%u], name[%s]", m_ID, name.c_str());
			/*LOGD("name[%s], val[%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f]", name.c_str(),
				 mat[0].x, mat[0].y, mat[0].z, mat[0].w,
				 mat[1].x, mat[1].y, mat[1].z, mat[1].w,
				 mat[2].x, mat[2].y, mat[2].z, mat[2].w,
				 mat[3].x, mat[3].y, mat[3].z, mat[3].w);*/
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetVec2(const std::string& name, const glm::vec2& vec2)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniform2f(ind, vec2.x, vec2.y);
			GLHelper::CheckError("SetVec2 err, program[%u], name[%s], val[%f, %f]", m_ID, name.c_str(), vec2.x, vec2.y);
			//LOGD("name[%s], val[%f, %f]", name.c_str(), vec2.x, vec2.y);
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetVec3(const std::string& name, const glm::vec3& vec3)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniform3f(ind, vec3.x, vec3.y, vec3.z);
			GLHelper::CheckError("SetVec3 err, program[%u], name[%s], val[%f, %f, %f]", m_ID, name.c_str(), vec3.x, vec3.y, vec3.z);
			//LOGD("name[%s], val[%f, %f, %f]", name.c_str(), vec3.x, vec3.y, vec3.z);
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetVec4(const std::string& name, const glm::vec4& vec4)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniform4f(ind, vec4.x, vec4.y, vec4.z, vec4.w);
			GLHelper::CheckError("SetVec4 err, program[%u], name[%s], val[%f, %f, %f£¬ %f]", m_ID, name.c_str(), vec4.x, vec4.y, vec4.z, vec4.w);
			//LOGD("name[%s], val[%f, %f, %f, %f]", name.c_str(), vec4.x, vec4.y, vec4.z, vec4.w);
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetIVec2(const std::string& name, const glm::ivec2& vec2)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniform2i(ind, vec2.x, vec2.y);
			GLHelper::CheckError("SetIVec2 err, program[%u], name[%s], val[%d, %d]", m_ID, name.c_str(), vec2.x, vec2.y);
			//LOGD("name[%s], val[%d, %d]", name.c_str(), vec2.x, vec2.y);
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetIVec3(const std::string& name, const glm::ivec3& vec3)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniform3i(ind, vec3.x, vec3.y, vec3.z);
			GLHelper::CheckError("SetIVec3 err, program[%u], name[%s], val[%d, %d, %d]", m_ID, name.c_str(), vec3.x, vec3.y, vec3.z);
			//LOGD("name[%s], val[%d, %d, %d]", name.c_str(), vec3.x, vec3.y, vec3.z);
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetIVec4(const std::string& name, const glm::ivec4& vec4)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniform4i(ind, vec4.x, vec4.y, vec4.z, vec4.w);
			GLHelper::CheckError("SetIVec4 err, program[%u], name[%s], val[%d, %d, %d, %d]", m_ID, name.c_str(), vec4.x, vec4.y, vec4.z, vec4.w);
			//LOGD("name[%s], val[%d, %d, %d, %d]", name.c_str(), vec4.x, vec4.y, vec4.z, vec4.w);
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	void GLShader::SetTextureSampler(const std::string& name, TextureSampler slot)
	{
		if (IsInit())
		{
			GLint ind = glGetUniformLocation(m_ID, name.c_str());
			glUniform1i(ind, (int32_t)slot);
			m_OccupiedSlots.set((int32_t)slot);
			GLHelper::CheckError("SetTextureSampler err, program[%u], name[%s], slot[%d]",
								 m_ID, name.c_str(), (int32_t)slot);
			//LOGD("name[%s], slot[%d]", name.c_str(), (int32_t)slot);
		}
		else
		{
			LOGE("shader[%s] hasn't been init yet", m_Name.c_str());
		}
	}

	bool GLShader::Init()
	{
		if (IsInit())
			return true;

		if(m_StageSource.empty())
		{
			LOGE("shader[%s]'s stage source is empty", m_Name.c_str());
			return false;
		}

		std::string vertSource, fragSource, geoSource;
		for(auto& [stage, stageData] : m_StageSource)
		{
			switch(stage)
			{
				case ShaderUtils::SHADER_STAGE_VERTEX_BIT:
				{
					vertSource = stageData.Source;
					break;
				}
				case ShaderUtils::SHADER_STAGE_GEOMETRY_BIT:
				{
					geoSource = stageData.Source;
					break;
				}
				case ShaderUtils::SHADER_STAGE_FRAGMENT_BIT:
				{
					fragSource = stageData.Source;
					break;
				}
				default:
					break;
			}
		}

		m_ID = GLHelper::CreateSimpleProgram(m_StageSource.size(), { vertSource, fragSource, geoSource });		
		bool success = m_ID > 0;
		if (success)
		{
			LOGD("succeed to init shader[%s][%u]", m_Name.c_str(), m_ID);
		}
		return success;
	}

	void GLShader::Destroy()
	{
		if (IsInit())
		{
			glDeleteProgram(m_ID);
			m_ID = 0;
			FreeSamplerUnit();
		}
	}

	void GLShader::Bind()
	{
		Init();

		if (IsInit())
		{
			glUseProgram(m_ID);
			FreeSamplerUnit();
			m_Bound = GLHelper::CheckError("err! program[%u]", m_ID);
		}
	}

	void GLShader::UnBind()
	{
		glUseProgram(0);
		m_Bound = false;
	}

	bool GLShader::GetSampler(TextureSampler& out) const
	{
		bool found{ false };
		if (m_OccupiedSlots.any())
		{
			for (uint8_t i = 0; i < (uint8_t)TextureSampler::MaxSlotNum; i++)
			{
				if (!m_OccupiedSlots.test(i))
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
			LOGE("no valid sampler for shader[%s]", m_Name.c_str());

		return found;
	}

	GLShaderMutant::GLShaderMutant(const Ref<RenderContext>& context, const std::string& name, 
								   const std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource)
		: ShaderMutant(context, name)
	{
		m_Shader = CreateRef<GLShader>(context, m_Name, stageSource);
	}
}