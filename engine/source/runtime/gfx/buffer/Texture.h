#ifndef __PIONNER_GFX_BUFFER_TEXTURE_H__
#define __PIONNER_GFX_BUFFER_TEXTURE_H__

#include <string>
#include <atomic>

#include "Buffer.h"

namespace pio
{
	namespace gfx
	{
		class Texture : public Buffer
		{
		public:
			static uint32_t textureType(TextureType type);

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
			TextureType  m_texType{ TEXTURE_TYPE_CNT };
			std::string  m_srcPath{};
			TextureParam m_param{};
			int32_t m_width{ 0 }, m_height{ 0 };
			int32_t m_channel{ 0 };
		};

		template<>
		bool Buffer::is<Texture>();

		template<>
		Texture *Buffer::as<Texture>();
	}
}

#endif