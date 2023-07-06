#ifndef __PIONNER_GFX_BUFFER_TEXTURE_H__
#define __PIONNER_GFX_BUFFER_TEXTURE_H__

#include <string>
#include <atomic>

#include "Buffer.h"

namespace pio
{
	namespace gfx
	{
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
			TextureFilterMin minFilter{MIN_FILTER_LINEAR};
			TextureFilterMag magFilter{MAG_FILTER_LINEAR};
			TextureWrap wrapS{ WRAP_CLAMP_TO_EDGE };
			TextureWrap wrapT{ WRAP_CLAMP_TO_EDGE };
			TextureWrap wrapR{ WRAP_CLAMP_TO_EDGE };
		};

		class Texture : public Buffer
		{
		public:
			Texture();
			Texture(TextureType type);
			virtual ~Texture();

			inline void setParam(const TextureParam &param) { m_param = param; }
			inline int32_t getWidth()       { return m_width; }
			inline int32_t getHeight()      { return m_height; }
			inline int32_t getChannel()     { return m_channel; }
			inline TextureType getTexType() { return m_texType; }

			void setPath(const std::string &path);

		protected:
			static uint32_t getMinFilter(TextureFilterMin filter);
			static uint32_t getMagFilter(TextureFilterMag filter);
			static uint32_t getWrap(TextureWrap wrap);

		protected:
			uint8_t *loadImgData();
			void freeImgData(uint8_t *data);

		protected:
			TextureType m_texType{ TEXTURE_TYPE_CNT };
			std::string m_srcPath{};
			TextureParam m_param{};
			int32_t m_width{ 0 }, m_height{ 0 };
			int32_t m_channel{ 0 };
			std::atomic<bool> m_dataChange{ false };
		};

		template<>
		bool Buffer::is<Texture>();

		template<>
		Texture *Buffer::as<Texture>();
	}
}

#endif