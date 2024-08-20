#include "GLDef.h"

namespace pio
{
	const char *GetGLUniformTypeStr(GLUniformType type)
	{
		switch (type)
		{
			case GLUniformType::Bool:
				return "Bool";
			case GLUniformType::Int:
				return "Int";
			case GLUniformType::UInt:
				return "UInt";
			case GLUniformType::Float:
				return "Float";
			case GLUniformType::Vec2:
				return "Vec2";
			case GLUniformType::Vec3:
				return "Vec3";
			case GLUniformType::Vec4:
				return "Vec4";
			case GLUniformType::IVec2:
				return "IVec2";
			case GLUniformType::IVec3:
				return "IVec3";
			case GLUniformType::IVec4:
				return "IVec4";
			case GLUniformType::Mat3:
				return "Mat3";
			case GLUniformType::Mat4:
				return "Mat";
			default:
				return "None GLUniformType";
		}
	}
}