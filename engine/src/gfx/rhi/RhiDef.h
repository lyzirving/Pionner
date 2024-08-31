#ifndef __PIONNER_GFX_RHI_DEF_H__
#define __PIONNER_GFX_RHI_DEF_H__

#include "asset/Asset.h"

namespace pio
{
	enum FrameBufferBlockBit : uint8_t
	{
		FB_ColorBuffer_Bit = 1 << 0,
		FB_DepthBuffer_Bit = 1 << 1,
		FB_StencilBuffer_Bit = 1 << 2
	};

	enum ShaderSpecifier : uint8_t
	{
		ShaderSpec_Standard, 
		ShaderSpec_Num
	};

	enum class RenderResourceType : uint8_t
	{
		UBO, VBO, EBO, VAO, FBO, Texture, Shader, Num
	};

	enum class TextureSampler : uint8_t
	{
		Slot0 = 0,
		Slot1, Slot2, Slot3,
		Slot4, Slot5, Slot6,
		Slot7, Slot8, Slot9,
		MaxSlotNum, InvalidSlot
	};

	enum class TextureWrap : uint8_t
	{
		ClampEdge, ClampBorder, Repeat
	};

	enum class TextureFilterMin : uint8_t
	{
		Nearest, Linear, Linear_Mipmap_Linear, Linear_Mipmap_Nearest, Nearest_Mipmap_Linear
	};

	enum class TextureFilterMag : uint8_t
	{
		Nearest, Linear
	};

	enum class TextureFormat : uint8_t
	{
		RGBA_32,    //8 bits per channel
		RGB_24,     //8 bits per channel
		RGB_16,     //integer values in the range [0, 65535] per channel
		R_16,       //integer values in the range [0, 65535] per channel
		R_8, 
		Alpha_8,
		RGBA_FLOAT, 
		RGBA_HALF,  //16 bits floating point per channel
		RG_FLOAT, 
		R_FLOAT,
		DEPTH_16,	//16 bits for a single channel depth map
		DEPTH_24,   //24 bits for a single channel depth map
		DEPTH_32,   //32 bits for a single channel depth map
		DEPTH_32F,  //32 bits as high resolution float for a single channel depth map
		DEPTH_24_STENCIL_8 // 24 bits for depth channel and 8 bits for stencil channel
	};

	enum class ShaderDataType : uint8_t
	{
		Byte, Byte2, Byte3, Byte4,
		UByte, UByte2, UByte3, UByte4,
		Short, Short2, Short3, Short4,
		UShort, UShort2, UShort3, UShort4,
		Int, Int2, Int3, Int4,
		UInt, UInt2, UInt3, UInt4,
		Float, Float2, Float3, Float4,
		Double, Double2, Double3, Double4
	};

	enum class UniformDataType : uint8_t
	{
		Bool, Float, Int, UInt,
		Vec2, Vec3, Vec4, IVec2, IVec3, IVec4, UVec2, UVec3, UVec4,
		Mat2, Mat3, Mat4,
		FloatArray, IntArray, UIntArray,
		Vec2Array, Vec3Array, Vec4Array,
		Mat2Array, Mat3Array, Mat4Array,
		Struct, StructArray
	};

	enum class BufferUsage : uint8_t
	{
		Static, Dynamic, DynamicRead
	};

	enum FrameBufferUsage : uint8_t
	{
		FrameBufferUsage_Color   = 0b00000001,
		FrameBufferUsage_Depth   = 0b00000010,
		FrameBufferUsage_Stencil = 0b00000100
	};

	enum class FrameBufferAttach : uint8_t
	{
		ColorAttach0, ColorAttach1, ColorAttach2, ColorAttach3, ColorAttach4, ColorAttach5, ColorAttach6, ColorAttach7, ColorAttachNum,
		DepthAttach
	};

	namespace Rhi
	{
		uint32_t GetShaderDataTypeByteSize(ShaderDataType type);
		uint32_t GetShaderDataTypeCompNum(ShaderDataType type);
		uint32_t GetUniformBaseAlign(UniformDataType type);
		uint32_t GetUniformColumnsByteSize(UniformDataType type);
		uint32_t GetUniformByteSize(UniformDataType type, uint32_t arrayNum = 1);
		const char* GetUniformDataTypeStr(UniformDataType type);
		const char* RenderResourceTypeStr(RenderResourceType type);
		uint32_t GetTextureChannelNum(TextureFormat fmt);
		uint32_t GetTextureByteSize(TextureFormat fmt);
	}
}

#endif