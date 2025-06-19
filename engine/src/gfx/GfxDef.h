#pragma once

#include "asset/def/AssetDef.h"

namespace pio
{
// Max point lit num allowed in the scene
#define PIO_POINT_LIT_LIMIT  (4)
// Dir lit shadow bias
#define PIO_MAX_DIR_LIT_SHADOW_BIAS (0.008f)

	namespace GpuAttr
	{
		namespace Tex
		{
			//Default texture
			static const char* WHITE = "WhiteTexture";
			static const char* BLACK = "BlackTexture";
			static const char* DIST_LIGHT = "distant_light";
			static const char* PT_LIGHT = "point_light";			
			static const char* DEFAULT_METALLIC_ROUGHNESS = "DefaultMetallicRoughness";

			static const char* ICON_MESH = "icon_mesh";
			static const char* ICON_MAT = "icon_mat";
			static const char* ICON_PIC = "icon_pic";
			static const char* ICON_DIR = "icon_directory";
			static const char* ICON_FILE = "icon_file";
			static const char* ICON_SHADER = "icon_shader";
			static const char* ICON_UNKNOWN = "icon_unknown";
			static const char* ICON_BACK = "icon_back";
			static const char* ICON_BACK_DISENABLE = "icon_back_disenable";
		}

		//Uniform data		
		static const char* UNI_SHADOW_MAP = "u_ShadowMap";
		static const char* UNI_PTLIT_SHADOW_MAP = "u_PointLitsShadow";

		//GBuffer in Deferred pass
		static const char* UNI_GBUFFER_POS      = "u_GPosition";
		static const char* UNI_GBUFFER_NORMAL   = "u_GNormal";
		static const char* UNI_GBUFFER_ALBEDO   = "u_GAlbedoAlpha";
		static const char* UNI_GBUFFER_MATERIAL = "u_GMaterial";		
		static const char* UNI_GBUFFER_MATERIAL_SUB = "u_GMaterialSub";

		static const char* UNI_COLOR_BUFFER = "u_ColorBuffer";
	}

	enum RenderingMode : uint8_t
	{
		RenderingMode_Opaque = 0,
		RenderingMode_Transparent,
		RenderingMode_Overlay,
		RenderingMode_Num
	};

	enum ShadowMode : uint8_t
	{
		ShadowMode_None = 0, 
		ShadowMode_Hard, 
		ShadowMode_Soft, 
		ShadowMode_Soft_2X, 
		ShadowMode_Soft_4X, 
		ShadowMode_Num
	};

	enum DrawMode : uint8_t
	{
		DrawMode_Triangle,
		DrawMode_Triangle_Strip, 
		DrawMode_Triangle_Fan,
		DrawMode_Line,
		DrawMode_Line_Strip,
		DrawMode_Line_Loop
	};

	enum FrameBufferBlockBit : uint8_t
	{
		FB_ColorBuffer_Bit = 1 << 0,
		FB_DepthBuffer_Bit = 1 << 1,
		FB_StencilBuffer_Bit = 1 << 2
	};

	enum FrameBufferUsage : uint8_t
	{
		FrameBufferUsage_Color = 0b00000001,
		FrameBufferUsage_Depth = 0b00000010,
		FrameBufferUsage_Stencil = 0b00000100
	};

	enum ShadowCastMode : uint8_t
	{
		ShadowCastMode_Off,
		ShadowCastMode_On,
		ShadowCastMode_TwoSided,
		ShadowCastMode_ShadowOnly,
		ShadowCastMode_Num
	};

	enum UBBinding : uint8_t
	{
		UBBinding_Camera = 0,
		UBBinding_PBRMaterial,
		UBBinding_TexturedMaterial,
		UBBinding_DirectionalLit,
		UBBinding_DirectionalLitShadow,
		UBBinding_PointLit,
		UBBinding_PointLitShadow,
		UBBinging_Motion,
		UBBinging_Skeleton,
		UBBinding_Num
	};

	enum ShaderSpecifier : uint8_t
	{
		SHADER_SPEC_MAIN_LIT_SHADOW_CASTER,
		SHADER_SPEC_POINT_LIT_SHADOW_CASTER,
		SHADER_SPEC_STANDARD,
		SHADER_SPEC_DEFERRED,
		SHADER_SPEC_POSTPROCESS,
		SHADER_SPEC_TEXTURED,
		SHADER_SPEC_FORWARD,
		SHADER_SPEC_NONE
	};

	enum class ShaderPermutationFlag : uint32_t
	{
		USE_SKELETON = 1 << 0,
		USE_CLEARCOAT = 1 << 1,
		NUM = 2
	};

	namespace ShaderMacro
	{
		constexpr const char* USE_SKELETON = "USE_SKELETON";
		constexpr const char* USE_CLEARCOAT = "USE_CLEARCOAT";

		inline const char* Get(ShaderPermutationFlag flag)
		{
			switch(flag)
			{
				case ShaderPermutationFlag::USE_SKELETON:
					return USE_SKELETON;
				case ShaderPermutationFlag::USE_CLEARCOAT:
					return USE_CLEARCOAT;
				default:
					LOGE("err! invalid flag[%u]", (uint32_t)flag);
					std::abort();
					return "";
			}
		}
	}

	enum class RenderingStrategy : uint8_t
	{
		Forward = 0, 
		Deffered
	};

	enum class LightType : uint8_t
	{
		DirectionLight,
		PointLight,
		Num
	};

	static constexpr uint8_t CASCADE_NUM = 3;

	enum class LightTech : uint8_t
	{
		ShadowMap,
		CascadeShadowMap,
		PointLitShadowMap,
		Num
	};

	enum class TextureType : uint8_t
	{
		TwoDimen, 
		SingleChannel,
		NormalMap, 
		RenderBuffer,
		CubeMap, 
		CubeMapArray,
		Num
	};

	// NOTE: definition order should not be changed
	enum CubeMapDir : uint8_t
	{
		CubeMapDir_PositiveX = 0,  // Right
		CubeMapDir_NegativeX,      // Left
		CubeMapDir_PositiveY,      // Top
		CubeMapDir_NegativeY,      // Bottom
		CubeMapDir_NegativeZ,      // Back.  In OpenGL, it is marked as GL_TEXTURE_CUBE_MAP_POSITIVE_Z	
		CubeMapDir_PositiveZ,      // Front. In OpenGL, it is marked as GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		CubeMapDir_Num
	};	

	enum class GizmoType : uint8_t
	{
		Wireframe
	};	

	enum class RenderResourceType : uint8_t
	{
		UBO, VBO, EBO, VAO, FBO, Texture, RenderBuffer, Shader, FBOProxy, UBOProxy, Num
	};

	enum class TextureSampler : uint8_t
	{
		Slot0 = 0,
		Slot1, Slot2, Slot3,
		Slot4, Slot5, Slot6,
		Slot7, Slot8, Slot9,
		MaxSlotNum, 
		InvalidSlot
	};

	//Only three types are allowed
	enum class IndexInternalFmt : uint8_t
	{
		U_BYTE, U_SHORT, U_INT
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

	enum class UniformType : uint8_t
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
		Static, 
		Dynamic,
		DynamicRead
	};

	enum class FrameBufferAttach : uint8_t
	{
		ColorAttach0, 
		ColorAttach1, 
		ColorAttach2, 
		ColorAttach3, 
		ColorAttach4, 
		ColorAttach5, 
		ColorAttach6, 
		ColorAttach7, 
		ColorAttachNum,
		DepthAttach
	};

	struct RenderConfig
	{
		glm::vec4 ClearColor{ 0.f, 0.f, 0.f, 1.f };
		RenderingStrategy Strategy{ RenderingStrategy::Deffered };	
	};

	uint32_t GetShaderDataTypeByteSize(ShaderDataType type);
	uint32_t GetShaderDataTypeCompNum(ShaderDataType type);
	uint32_t GetUniformBaseAlign(UniformType type);
	uint32_t GetUniformColumnsByteSize(UniformType type);
	uint32_t GetUniformByteSize(UniformType type, uint32_t arrayNum = 1);
	const char* GetUniformDataTypeStr(UniformType type);
	const char* RenderResourceTypeStr(RenderResourceType type);

	template<typename T>
	IndexInternalFmt GetIndexInternalFmt()
	{
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Rhi"
		LOGE("err! invalid index internal format");
		std::abort();
		return IndexInternalFmt::U_BYTE;
	};
}