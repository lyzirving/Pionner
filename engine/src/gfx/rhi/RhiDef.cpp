#include "RhiDef.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RhiDef"

namespace pio
{
	namespace Rhi
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

		uint32_t GetUniformBaseAlign(UniformDataType type)
		{
			const uint32_t N = 4;
			switch (type)
			{
				case UniformDataType::Bool:
				case UniformDataType::Float:
				case UniformDataType::Int:
				case UniformDataType::UInt:
					return N;
				case UniformDataType::Vec2:
				case UniformDataType::IVec2:
				case UniformDataType::UVec2:
					return 2 * N;
				case UniformDataType::Vec3:
				case UniformDataType::Vec4:
				case UniformDataType::IVec3:
				case UniformDataType::IVec4:
				case UniformDataType::UVec3:
				case UniformDataType::UVec4:
					return 4 * N;
					// For each column in Mat
				case UniformDataType::Mat2:
				case UniformDataType::Mat3:
				case UniformDataType::Mat4:
					return 4 * N;
					// For each item in array
				case UniformDataType::FloatArray:
				case UniformDataType::IntArray:
				case UniformDataType::UIntArray:
				case UniformDataType::Vec2Array:
				case UniformDataType::Vec3Array:
				case UniformDataType::Vec4Array:
				case UniformDataType::Mat2Array:
				case UniformDataType::Mat3Array:
				case UniformDataType::Mat4Array:
					return 4 * N;
				case UniformDataType::Struct:
				case UniformDataType::StructArray:
					return 4 * N;
				default:
					LOGE("err! invalid data type[%u]", type);
					std::abort();
					return 0;
			}
		}

		uint32_t GetUniformColumnsByteSize(UniformDataType type)
		{
			const uint32_t N = 4;
			switch (type)
			{
				case UniformDataType::Bool:
				case UniformDataType::Float:
				case UniformDataType::Int:
				case UniformDataType::UInt:
				case UniformDataType::FloatArray:
				case UniformDataType::IntArray:
				case UniformDataType::UIntArray:
					return N;
				case UniformDataType::Vec2:
				case UniformDataType::IVec2:
				case UniformDataType::UVec2:
				case UniformDataType::Vec2Array:
					return 2 * N;
				case UniformDataType::Vec3:
				case UniformDataType::IVec3:
				case UniformDataType::UVec3:
				case UniformDataType::Vec3Array:
					return 3 * N;
				case UniformDataType::Vec4:
				case UniformDataType::IVec4:
				case UniformDataType::UVec4:
				case UniformDataType::Vec4Array:
					return 4 * N;
				case UniformDataType::Mat2:
				case UniformDataType::Mat2Array:
					return 2 * N;
				case UniformDataType::Mat3:
				case UniformDataType::Mat3Array:
					return 3 * N;
				case UniformDataType::Mat4:
				case UniformDataType::Mat4Array:
					return 4 * N;
				default:
					LOGE("err! invalid data type[%u]", type);
					std::abort();
					return 0;
			}
		}

		uint32_t GetUniformByteSize(UniformDataType type, uint32_t arrayNum)
		{
			switch (type)
			{
				case UniformDataType::Bool:
					return 1;
				case UniformDataType::Float:
				case UniformDataType::Int:
				case UniformDataType::UInt:
					return 4;
				case UniformDataType::FloatArray:
				case UniformDataType::IntArray:
				case UniformDataType::UIntArray:
					return 4 * arrayNum;
				case UniformDataType::Vec2:
				case UniformDataType::IVec2:
				case UniformDataType::UVec2:
					return 8;
				case UniformDataType::Vec2Array:
					return 8 * arrayNum;
				case UniformDataType::Vec3:
				case UniformDataType::IVec3:
				case UniformDataType::UVec3:
					return 12;
				case UniformDataType::Vec3Array:
					return 12 * arrayNum;
				case UniformDataType::Vec4:
				case UniformDataType::IVec4:
				case UniformDataType::UVec4:
					return 16;
				case UniformDataType::Vec4Array:
					return 16 * arrayNum;
				case UniformDataType::Mat2:
					return 2 * 2 * 4;
				case UniformDataType::Mat2Array:
					return 4 * 4 * arrayNum;
				case UniformDataType::Mat3:
					return 3 * 3 * 4;
				case UniformDataType::Mat3Array:
					return 3 * 3 * 4 * arrayNum;
				case UniformDataType::Mat4:
					return 4 * 4 * 4;
				case UniformDataType::Mat4Array:
					return 4 * 4 * 4 * arrayNum;
				default:
					LOGE("err! invalid data type[%u]", type);
					std::abort();
					return 0;
			}
		}

		const char* GetUniformDataTypeStr(UniformDataType type)
		{
			switch (type)
			{
				case UniformDataType::Bool:
					return "bool";
				case UniformDataType::Float:
					return "float";
				case UniformDataType::Int:
					return "int";
				case UniformDataType::UInt:
					return "uint";
				case UniformDataType::Vec2:
					return "vec2";
				case UniformDataType::Vec3:
					return "vec3";
				case UniformDataType::Vec4:
					return "vec4";
				case UniformDataType::IVec2:
					return "ivec2";
				case UniformDataType::IVec3:
					return "ivec3";
				case UniformDataType::IVec4:
					return "ivec4";
				case UniformDataType::UVec2:
					return "uvec2";
				case UniformDataType::UVec3:
					return "uvec3";
				case UniformDataType::UVec4:
					return "uvec4";
				case UniformDataType::Mat2:
					return "mat2";
				case UniformDataType::Mat3:
					return "mat3";
				case UniformDataType::Mat4:
					return "mat4";
				case UniformDataType::FloatArray:
					return "float array";
				case UniformDataType::IntArray:
					return "int array";
				case UniformDataType::UIntArray:
					return "uint array";
				case UniformDataType::Vec2Array:
					return "vec2 array";
				case UniformDataType::Vec3Array:
					return "vec3 array";
				case UniformDataType::Vec4Array:
					return "vec4 array";
				case UniformDataType::Mat2Array:
					return "mat2 array";
				case UniformDataType::Mat3Array:
					return "mat3 array";
				case UniformDataType::Mat4Array:
					return "mat4 array";
				case UniformDataType::Struct:
					return "struct";
				case UniformDataType::StructArray:
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
				default:
					LOGE("err! undefined render resource type[%u]", type);
					std::abort();
					return "Undefined RenderResourceType";
			}
		}

		uint32_t GetTextureChannelNum(TextureFormat fmt)
		{
			switch (fmt)
			{
				case TextureFormat::RGBA_32:
				case TextureFormat::RGBA_FLOAT:
					return 4;
				case TextureFormat::RGB_24:
				case TextureFormat::RGBA_HALF:
				case TextureFormat::RGB_16:
					return 3;
				case TextureFormat::RG_FLOAT:
				case TextureFormat::DEPTH_24_STENCIL_8:
					return 2;
				case TextureFormat::R_16:
				case TextureFormat::R_8:
				case TextureFormat::R_FLOAT:
				case TextureFormat::Alpha_8:
				case TextureFormat::DEPTH_16:
				case TextureFormat::DEPTH_24:
				case TextureFormat::DEPTH_32:
				case TextureFormat::DEPTH_32F:
					return 1;
				default:
					LOGE("err! texture format[%u] has not been implemented", fmt);
					std::abort();
					return 0;
			}
		}

		uint32_t GetTextureByteSize(TextureFormat fmt)
		{
			switch (fmt)
			{				
				case TextureFormat::RGBA_FLOAT:
				case TextureFormat::RG_FLOAT:
				case TextureFormat::R_FLOAT:
				case TextureFormat::DEPTH_32:
				case TextureFormat::DEPTH_32F:
				case TextureFormat::DEPTH_24_STENCIL_8:
					return 4;
				case TextureFormat::DEPTH_24:
					return 3;
				case TextureFormat::RGBA_HALF:
				case TextureFormat::RGB_16:
				case TextureFormat::R_16:
				case TextureFormat::DEPTH_16:
					return 2;
				case TextureFormat::RGBA_32:
				case TextureFormat::RGB_24:
				case TextureFormat::R_8:
				case TextureFormat::Alpha_8:
					return 1;
				default:
					LOGE("err! texture format[%u] has not been implemented", fmt);
					std::abort();
					return 0;
			}
		}
	}
}