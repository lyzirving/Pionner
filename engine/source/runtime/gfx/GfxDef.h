#ifndef __PIONNER_GFX_DEF_H__
#define __PIONNER_GFX_DEF_H__

#include <cstdint>
#include <memory>

namespace pio
{
	class WindowSystem;

	namespace gfx
	{
		enum GfxBufType
		{
			GFX_BUF_VERTEX,
			GFX_BUF_INDICE,
			GFX_BUF_TEXTURE,
			GFX_BUF_CUBE_TEXTURE,
			GFX_BUF_DEPTH_FBO,
			GFX_BUF_CUBE_DEPTH_FBO,
			GFX_BUF_COLOR_FRAMEBUFFER,
			GFX_BUF_CNT
		};

		enum ShapeType : uint8_t
		{
			SHAPE_BOUNDING_BOX,
			SHAPE_CNT
		};

		enum DataType : uint8_t
		{
			DATA_TYPE_UNSIGNED_BYTE,
			DATA_TYPE_UNSIGNED_SHORT,
			DATA_TYPE_UNSIGNED_INT
		};

		enum TextureType : uint8_t
		{
			TEXTURE_TYPE_1D,
			TEXTURE_TYPE_2D,
			TEXTURE_TYPE_CNT
		};

		enum TextureFilterMin : uint8_t
		{
			MIN_FILTER_NEAREST,
			MIN_FILTER_LINEAR,
			MIN_FILTER_NEAREST_MIPMAP_NEAREST,
			MIN_FILTER_LINEAR_MIPMAP_NEAREST,
			MIN_FILTER_NEAREST_MIPMAP_LINEAR,
			MIN_FILTER_LINEAR_MIPMAP_LINEAR
		};

		enum TextureFilterMag : uint8_t
		{
			MAG_FILTER_NEAREST,
			MAG_FILTER_LINEAR
		};

		enum TextureWrap : uint8_t
		{
			WRAP_REPEAT,
			WRAP_CLAMP_TO_EDGE,
			WRAP_CLAMP_TO_BORDER,
			WRAP_MIRROED_REPEAT,
			WRAP_MIRROR_CLAMP_TO_EDGE
		};

		struct TextureParam
		{
			TextureFilterMin minFilter{ MIN_FILTER_LINEAR };
			TextureFilterMag magFilter{ MAG_FILTER_LINEAR };
			TextureWrap wrapS{ WRAP_CLAMP_TO_EDGE };
			TextureWrap wrapT{ WRAP_CLAMP_TO_EDGE };
			TextureWrap wrapR{ WRAP_CLAMP_TO_EDGE };
		};

		struct GfxContextInitParam
		{
			std::shared_ptr<WindowSystem> window{ nullptr };
		};
	}
}

#endif