#include "GfxDef.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxDef"

namespace pio
{
	uint32_t GetShaderDataTypeByteSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Byte:    return 1;
			case ShaderDataType::Byte2:   return 2;
			case ShaderDataType::Byte3:   return 3;
			case ShaderDataType::Byte4:   return 4;
			case ShaderDataType::UByte:   return 1;
			case ShaderDataType::UByte2:  return 2;
			case ShaderDataType::UByte3:  return 3;
			case ShaderDataType::UByte4:  return 4;
			case ShaderDataType::Short:   return 2;
			case ShaderDataType::Short2:  return 4;
			case ShaderDataType::Short3:  return 6;
			case ShaderDataType::Short4:  return 8;
			case ShaderDataType::UShort:  return 2;
			case ShaderDataType::UShort2: return 4;
			case ShaderDataType::UShort3: return 6;
			case ShaderDataType::UShort4: return 8;
			case ShaderDataType::Int:     return 4;
			case ShaderDataType::Int2:    return 8;
			case ShaderDataType::Int3:    return 12;
			case ShaderDataType::Int4:    return 16;
			case ShaderDataType::UInt:    return 4;
			case ShaderDataType::UInt2:   return 8;
			case ShaderDataType::UInt3:   return 12;
			case ShaderDataType::UInt4:   return 16;
			case ShaderDataType::Float:   return 4;
			case ShaderDataType::Float2:  return 8;
			case ShaderDataType::Float3:  return 12;
			case ShaderDataType::Float4:  return 16;
			case ShaderDataType::Double:  return 8;
			case ShaderDataType::Double2: return 16;
			case ShaderDataType::Double3: return 24;
			case ShaderDataType::Double4: return 32;
			default:
				LOGE("invalid type[%u] for data type byte size", type);
				std::abort();
				return 0;
		}
	}

	uint32_t GetShaderDataTypeCompNum(ShaderDataType type)
	{
		switch (type)
		{
			// ----------- Component 1 ----------------
			case ShaderDataType::Byte:
			case ShaderDataType::UByte:
			case ShaderDataType::Short:
			case ShaderDataType::UShort:
			case ShaderDataType::Int:
			case ShaderDataType::UInt:
			case ShaderDataType::Float:
			case ShaderDataType::Double:
				return 1;
				// ----------- Component 2 ----------------
			case ShaderDataType::Byte2:
			case ShaderDataType::UByte2:
			case ShaderDataType::Short2:
			case ShaderDataType::UShort2:
			case ShaderDataType::Int2:
			case ShaderDataType::UInt2:
			case ShaderDataType::Float2:
			case ShaderDataType::Double2:
				return 2;
				// ----------- Component 3 ----------------
			case ShaderDataType::Byte3:
			case ShaderDataType::UByte3:
			case ShaderDataType::Short3:
			case ShaderDataType::UShort3:
			case ShaderDataType::Int3:
			case ShaderDataType::UInt3:
			case ShaderDataType::Float3:
			case ShaderDataType::Double3:
				return 3;
				// ----------- Component 4 ----------------
			case ShaderDataType::Byte4:
			case ShaderDataType::UByte4:
			case ShaderDataType::Short4:
			case ShaderDataType::UShort4:
			case ShaderDataType::Int4:
			case ShaderDataType::UInt4:
			case ShaderDataType::Float4:
			case ShaderDataType::Double4:
				return 4;
			default:
				LOGE("invalid type[%u] for component", type);
				std::abort();
				return 0;
		}
	}

	uint32_t GetUniformBaseAlign(UniformType type)
	{
		const uint32_t N = 4;
		switch (type)
		{
			case UniformType::Bool:
			case UniformType::Float:
			case UniformType::Int:
			case UniformType::UInt:
				return N;
			case UniformType::Vec2:
			case UniformType::IVec2:
			case UniformType::UVec2:
				return 2 * N;
			case UniformType::Vec3:
			case UniformType::Vec4:
			case UniformType::IVec3:
			case UniformType::IVec4:
			case UniformType::UVec3:
			case UniformType::UVec4:
				return 4 * N;
				// For each column in Mat
			case UniformType::Mat2:
			case UniformType::Mat3:
			case UniformType::Mat4:
				return 4 * N;
				// For each item in array
			case UniformType::FloatArray:
			case UniformType::IntArray:
			case UniformType::UIntArray:
			case UniformType::Vec2Array:
			case UniformType::Vec3Array:
			case UniformType::Vec4Array:
			case UniformType::Mat2Array:
			case UniformType::Mat3Array:
			case UniformType::Mat4Array:
				return 4 * N;
			case UniformType::Struct:
			case UniformType::StructArray:
				return 4 * N;
			default:
				LOGE("err! invalid data type[%u]", type);
				std::abort();
				return 0;
		}
	}

	uint32_t GetUniformColumnsByteSize(UniformType type)
	{
		const uint32_t N = 4;
		switch (type)
		{
			case UniformType::Bool:
			case UniformType::Float:
			case UniformType::Int:
			case UniformType::UInt:
			case UniformType::FloatArray:
			case UniformType::IntArray:
			case UniformType::UIntArray:
				return N;
			case UniformType::Vec2:
			case UniformType::IVec2:
			case UniformType::UVec2:
			case UniformType::Vec2Array:
				return 2 * N;
			case UniformType::Vec3:
			case UniformType::IVec3:
			case UniformType::UVec3:
			case UniformType::Vec3Array:
				return 3 * N;
			case UniformType::Vec4:
			case UniformType::IVec4:
			case UniformType::UVec4:
			case UniformType::Vec4Array:
				return 4 * N;
			case UniformType::Mat2:
			case UniformType::Mat2Array:
				return 2 * N;
			case UniformType::Mat3:
			case UniformType::Mat3Array:
				return 3 * N;
			case UniformType::Mat4:
			case UniformType::Mat4Array:
				return 4 * N;
			default:
				LOGE("err! invalid data type[%u]", type);
				std::abort();
				return 0;
		}
	}

	uint32_t GetUniformByteSize(UniformType type, uint32_t arrayNum)
	{
		switch (type)
		{
			case UniformType::Bool:
				return 1;
			case UniformType::Float:
			case UniformType::Int:
			case UniformType::UInt:
				return 4;
			case UniformType::FloatArray:
			case UniformType::IntArray:
			case UniformType::UIntArray:
				return 4 * arrayNum;
			case UniformType::Vec2:
			case UniformType::IVec2:
			case UniformType::UVec2:
				return 8;
			case UniformType::Vec2Array:
				return 8 * arrayNum;
			case UniformType::Vec3:
			case UniformType::IVec3:
			case UniformType::UVec3:
				return 12;
			case UniformType::Vec3Array:
				return 12 * arrayNum;
			case UniformType::Vec4:
			case UniformType::IVec4:
			case UniformType::UVec4:
				return 16;
			case UniformType::Vec4Array:
				return 16 * arrayNum;
			case UniformType::Mat2:
				return 2 * 2 * 4;
			case UniformType::Mat2Array:
				return 4 * 4 * arrayNum;
			case UniformType::Mat3:
				return 3 * 3 * 4;
			case UniformType::Mat3Array:
				return 3 * 3 * 4 * arrayNum;
			case UniformType::Mat4:
				return 4 * 4 * 4;
			case UniformType::Mat4Array:
				return 4 * 4 * 4 * arrayNum;
			default:
				LOGE("err! invalid data type[%u]", type);
				std::abort();
				return 0;
		}
	}

	const char* GetUniformDataTypeStr(UniformType type)
	{
		switch (type)
		{
			case UniformType::Bool:
				return "bool";
			case UniformType::Float:
				return "float";
			case UniformType::Int:
				return "int";
			case UniformType::UInt:
				return "uint";
			case UniformType::Vec2:
				return "vec2";
			case UniformType::Vec3:
				return "vec3";
			case UniformType::Vec4:
				return "vec4";
			case UniformType::IVec2:
				return "ivec2";
			case UniformType::IVec3:
				return "ivec3";
			case UniformType::IVec4:
				return "ivec4";
			case UniformType::UVec2:
				return "uvec2";
			case UniformType::UVec3:
				return "uvec3";
			case UniformType::UVec4:
				return "uvec4";
			case UniformType::Mat2:
				return "mat2";
			case UniformType::Mat3:
				return "mat3";
			case UniformType::Mat4:
				return "mat4";
			case UniformType::FloatArray:
				return "float array";
			case UniformType::IntArray:
				return "int array";
			case UniformType::UIntArray:
				return "uint array";
			case UniformType::Vec2Array:
				return "vec2 array";
			case UniformType::Vec3Array:
				return "vec3 array";
			case UniformType::Vec4Array:
				return "vec4 array";
			case UniformType::Mat2Array:
				return "mat2 array";
			case UniformType::Mat3Array:
				return "mat3 array";
			case UniformType::Mat4Array:
				return "mat4 array";
			case UniformType::Struct:
				return "struct";
			case UniformType::StructArray:
				return "struct array";
			default:
				LOGE("err! invalid data type[%u]", type);
				std::abort();
				return "None";
		}
	}

	const char* RenderResourceTypeStr(RenderResourceType type)
	{
		switch (type)
		{
			case RenderResourceType::UBO:
				return "Uniform Buffer Object";
			case RenderResourceType::VBO:
				return "Vertex Buffer Object";
			case RenderResourceType::EBO:
				return "Indice Buffer Object";
			case RenderResourceType::VAO:
				return "Vertex Array Object";
			case RenderResourceType::FBO:
				return "Frame Buffer Object";
			case RenderResourceType::Texture:
				return "Texture";
			case RenderResourceType::RenderBuffer:
				return "Render Buffer";
			case RenderResourceType::Shader:
				return "Shader";
			case RenderResourceType::FBOProxy:
				return "FBOProxy";
			case RenderResourceType::UBOProxy:
				return "UBOProxy";
			default:
				LOGE("err! undefined render resource type[%u]", type);
				std::abort();
				return "Undefined RenderResourceType";
		}
	}
}