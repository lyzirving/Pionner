#ifndef __PIONNER_PLATFORM_OPENGL_GL_DEF_H__
#define __PIONNER_PLATFORM_OPENGL_GL_DEF_H__

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
		GLUniform(const std::string &name = "None", GLUniformType type = GLUniformType::Bool)
			: m_name(name), m_dataType(type) {}
		~GLUniform() = default;

		GLUniform(const GLUniform &rhs);
		GLUniform &operator=(const GLUniform &rhs);

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