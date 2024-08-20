#ifndef __PIONNER_GFX_RHI_OPENGL_GL_DEF_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_DEF_H__

#include "Base.h"

namespace pio
{
	enum class GLUniformType
	{
		Bool = 0, Int, UInt, Float,
		Vec2, Vec3, Vec4, IVec2, IVec3, IVec4,
		Mat3, Mat4,
	};

	class GLUniform
	{
	public:
		GLUniform(const std::string &name = "None", GLUniformType type = GLUniformType::Bool) : m_name(name), m_dataType(type) {}
		~GLUniform() = default;

		GLUniform(const GLUniform &rhs)
		{
			m_name = rhs.m_name;
			m_dataType = rhs.m_dataType;
			m_byteSize = rhs.m_byteSize;
			m_offset = rhs.m_offset;
		}

		GLUniform &operator=(const GLUniform &rhs)
		{
			if(this != &rhs)
			{
				this->GLUniform::GLUniform(rhs);
			}
			return *this;
		}

	private:
		std::string m_name;
		GLUniformType m_dataType;
		uint32_t m_byteSize{ 0 };
		uint32_t m_offset{ 0 };

	private:
		friend class GLMaterial;
		friend class GLRenderPass;
	};

	const char *GetGLUniformTypeStr(GLUniformType type);
}

#endif