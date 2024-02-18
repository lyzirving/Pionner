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
		None = 0, ClampEdge, ClampBorder, Repeat
	};

	enum class TextureFilterMin : uint8_t
	{
		None = 0, Linear, Nearest
	};

	enum class TextureFilterMag : uint8_t
	{
		None = 0, Linear, Nearest,
	};

	enum class TextureInternalFmt : uint8_t
	{
		None = 0,
		Depth, Depth24, Depth32F, DepthStencil,
		RED, R8, R8U, R16U, R32U, R32F,
		RG, RG8, RG8U, RG16F, RG32F,
		RGB, RGB8U, RGBUI, RGB16F, RGB32F,
		RGBA, RGBAUI, RGBA8U, RGBA16U, RGBA16F, RGBA32F,
	};

	enum class TextureFmt : uint8_t
	{
		None = 0,
		Depth, DepthStencil,
		RED, RG, RGB, BGR, RGBA, BGRA,
		REDI, RGI, RGBI, BGRI, RGBAI, BGRAI
	};

	enum class ShaderDataType : uint8_t
	{
		None = 0,
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
		None = 0,
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
		None = 0, Static, Dynamic,
		DynamicRead
	};

	enum class ColorAttachment : uint8_t
	{
		Attach0, Attach1, Attach2, Attach3, Attach4, Attach5, Attach6, Attach7, Num
	};

	enum class DepthAttachment : uint8_t
	{
		Depth, DepthStencil
	};

	enum class FrameBufferUsage : uint8_t
	{
		ColorBuffer = 0, DepthBuffer, DepthStencil
	};		

	enum class UiFocus : uint8_t
	{
		None = 0, MouseCapture, KeyboardCapture, TextInputCapture
	};

	enum class RenderStrategy : uint8_t
	{
		Forward = 0, Deferred
	};

	namespace RhiDef
	{
		uint32_t GetShaderDataTypeByteSize(ShaderDataType type);
		uint32_t GetShaderDataTypeCompNum(ShaderDataType type);
		uint32_t GetUniformBaseAlign(UniformDataType type);
		uint32_t GetUniformColumnsByteSize(UniformDataType type);
		uint32_t GetUniformByteSize(UniformDataType type, uint32_t arrayNum = 1);
		const char *GetUniformDataTypeStr(UniformDataType type);
		TextureInternalFmt GetInternalFmt(uint32_t channelNum);
		TextureFmt GetTextureFmt(uint32_t channelNum);
	}

	struct RendererDebugger
	{
		bool  Raycast{ false };
		float OutlineScale{ 0.02f };
		glm::vec4 OutlineColor{ 0.93f, 0.58f, 0.19f, 1.f };
	};

	struct RendererConfig
	{
		float FPS{ 0.f };
		uint64_t FrameTime{ 0 };	
		glm::vec4 ClearColor{ 0.3f, 0.3f, 0.3f, 1.f };
		RenderStrategy Strategy{ RenderStrategy::Deferred };
		AssetHandle FullScreenQuad{ NullAsset };
		RendererDebugger Debugger{};
	};

	#define PIO_UNIFORM_WRAPPER_INTERFACE_DECLARE(T) \
            public:\
            virtual void set(const std::string &name, float value) = 0;\
	        virtual void set(const std::string &name, int value) = 0;\
	        virtual void set(const std::string &name, uint32_t value) = 0;\
	        virtual void set(const std::string &name, bool value) = 0;\
	        virtual void set(const std::string &name, const glm::vec2 &value) = 0;\
	        virtual void set(const std::string &name, const glm::vec3 &value) = 0;\
	        virtual void set(const std::string &name, const glm::vec4 &value) = 0;\
	        virtual void set(const std::string &name, const glm::ivec3 &value) = 0;\
	        virtual void set(const std::string &name, const glm::ivec4 &value) = 0;\
	        virtual void set(const std::string &name, const glm::ivec2 &value) = 0;\
	        virtual void set(const std::string &name, const glm::mat3 &value) = 0;\
	        virtual void set(const std::string &name, const glm::mat4 &value) = 0;\
            \
            virtual bool getBool(const std::string &name) = 0;\
	        virtual float getFloat(const std::string &name) = 0;\
	        virtual int32_t getInt(const std::string &name) = 0;\
	        virtual uint32_t getUInt(const std::string &name) = 0;\
	        virtual glm::vec2 getVector2(const std::string &name) = 0;\
	        virtual glm::vec3 getVector3(const std::string &name) = 0;\
	        virtual glm::vec4 getVector4(const std::string &name) = 0;\
	        virtual glm::mat4 getMatrix4(const std::string &name) = 0;


	#define PIO_UNIFORM_WRAPPER_CLASS_DECLARE(T)\
            public:\
            virtual void set(const std::string &name, float value) override;\
	        virtual void set(const std::string &name, int value) override;\
	        virtual void set(const std::string &name, uint32_t value) override;\
	        virtual void set(const std::string &name, bool value) override;\
	        virtual void set(const std::string &name, const glm::vec2 &value) override;\
	        virtual void set(const std::string &name, const glm::vec3 &value) override;\
	        virtual void set(const std::string &name, const glm::vec4 &value) override;\
	        virtual void set(const std::string &name, const glm::ivec3 &value) override;\
	        virtual void set(const std::string &name, const glm::ivec4 &value) override;\
	        virtual void set(const std::string &name, const glm::ivec2 &value) override;\
	        virtual void set(const std::string &name, const glm::mat3 &value) override;\
	        virtual void set(const std::string &name, const glm::mat4 &value) override;\
            \
	        virtual float getFloat(const std::string &name) override;\
	        virtual int32_t getInt(const std::string &name) override;\
	        virtual uint32_t getUInt(const std::string &name) override;\
	        virtual bool getBool(const std::string &name) override;\
	        virtual glm::vec2 getVector2(const std::string &name) override;\
	        virtual glm::vec3 getVector3(const std::string &name) override;\
	        virtual glm::vec4 getVector4(const std::string &name) override;\
	        virtual glm::mat4 getMatrix4(const std::string &name) override;


}

#endif