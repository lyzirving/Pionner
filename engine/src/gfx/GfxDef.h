#ifndef __PIONNER_GFX_DEF_H__
#define __PIONNER_GFX_DEF_H__

#include "Common.h"

namespace pio
{
	//Extension of frustum clip for shadow
	#define SD_FRUST_EXT (100.f)

	namespace GpuAttr
	{
		namespace Mat
		{
			//Default material
			static const char* STANDARD = "StandardMaterial";
			static const char* SPRITE = "SpriteMaterial";
			static const char* COLOR = "ColorMaterial";
		}

		namespace Tex
		{
			//Default texture
			static const char* WHITE = "WhiteTexture";
			static const char* BLACK = "BlackTexture";
			static const char* DIST_LIGHT = "distant_light";
		}

		//Uniform data
		static const char* UNI_MODEL_MAT = "u_modelMat";
		//Uniform block
		static const char* BINDING_CAM_BLOCK = "CameraBlock";
		static const char* BINDING_DIRECTIONAL_LIGHT_BLOCK = "DirectionalLightBlock";
		static const char* BINDING_MAIN_LIGHT_CASTER_BLOCK = "MainLightCasterBlock";
		static const char* BINDING_CASCADE_SHADOW_MAP = "CascadeShadowMap";

		//GBuffer in Deferred pass
		static const char* UNI_GBUFFER_POS      = "u_GPosition";
		static const char* UNI_GBUFFER_NORMAL   = "u_GNormal";
		static const char* UNI_GBUFFER_ALBEDO   = "u_GAlbedoAlpha";
		static const char* UNI_GBUFFER_MATERIAL = "u_GMaterial";
		static const char* UNI_GBUFFER_EMISSION = "u_GEmission";

		static const char* UNI_COLOR_BUFFER = "u_colorBuffer";
		static const char* UNI_SHADOW_MAP = "u_shadowMap";
	}

	enum RenderingMode : uint8_t
	{
		RenderingMode_Opaque = 0,
		RenderingMode_Transparent,
		RenderingMode_Overlay,
		RenderingMode_Num
	};

	enum ProjectionType : uint8_t
	{
		ProjectionType_Perspective,
		ProjectionType_Orthographic,
		ProjectionType_Num
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

	enum ShaderSpecifier : uint8_t
	{
		ShaderSpec_Standard,
		ShaderSpec_Sprite,
		ShaderSpec_Color
	};

	enum ShadowCastMode : uint8_t
	{
		ShadowCastMode_Off,
		ShadowCastMode_On,
		ShadowCastMode_TwoSided,
		ShadowCastMode_ShadowOnly,
		ShadowCastMode_Num
	};

	enum class RenderingStrategy : uint8_t
	{
		Forward = 0, 
		Deffered
	};

	enum class MeshType : uint8_t
	{
		Plane, 
		Cube,
		Sphere,
		Outline,
		Num
	};

	const char* GetMeshTypeStr(MeshType type);
	const char* GetShaderSpecStr(ShaderSpecifier spec);
	const char* GetRenderingModeStr(RenderingMode mode);

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
		Num
	};

	enum class TextureType : uint8_t
	{
		TwoDimen, 
		SingleChannel,
		NormalMap, 
		RenderBuffer,
		CubeMap, 
		CubeArray,
		Num
	};

	enum class GizmoType : uint8_t
	{
		DirLight
	};

	enum class ShaderType : uint8_t
	{
		MainLightShadowCaster,
		GBuffer, 
		Deferred,
		ColorCorrect,
		Sprite,
		Outline,
		Num
	};

	enum class RenderResourceType : uint8_t
	{
		UBO, VBO, EBO, VAO, FBO, Texture, RenderBuffer, Shader, Proxy, Num
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
		ClampEdge, 
		ClampBorder,
		Repeat
	};

	enum class TextureFilterMin : uint8_t
	{
		Nearest, 
		Linear,
		Linear_Mipmap_Linear,
		Linear_Mipmap_Nearest,
		Nearest_Mipmap_Linear
	};

	enum class TextureFilterMag : uint8_t
	{
		Nearest,
		Linear
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
		RenderingStrategy Strategy{ RenderingStrategy::Deffered };
		LightTech LightingTech{ LightTech::ShadowMap };
		bool IndicateCSM{ false };
		glm::vec4 ClearColor{ 0.f, 0.f, 0.f, 1.f };
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
}

#endif