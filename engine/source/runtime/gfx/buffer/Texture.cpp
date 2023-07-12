#include <cstring>

#ifndef STB_IMAGE_STATIC
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_STATIC
#include <stb/stb_image.h>

#include "Texture.h"

#include "render/rhi/RhiHeader.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Texture"

namespace pio
{
	namespace gfx
	{
		uint32_t Texture::textureType(TextureType type)
		{
			switch (type)
			{
				case pio::gfx::TEXTURE_TYPE_1D:
					return GL_TEXTURE_1D;
				case pio::gfx::TEXTURE_TYPE_2D:
				default:
					return GL_TEXTURE_2D;
			}
		}

		Texture::Texture() : Buffer(GFX_BUF_TEXTURE)
		{
		}

		Texture::Texture(TextureType type) : Buffer(GFX_BUF_TEXTURE), m_texType(type)
		{
		}

		Texture::~Texture()
		{
		}

		void Texture::setPath(const std::string &path)
		{
			if (std::strcmp(path.c_str(), m_srcPath.c_str()) != 0)
			{
				m_dataChange.store(true);
				m_srcPath = path;
			}
		}

		uint32_t Texture::getMinFilter(TextureFilterMin filter)
		{
			switch (filter)
			{
				case pio::gfx::MIN_FILTER_NEAREST:
					return GL_NEAREST;
				case pio::gfx::MIN_FILTER_NEAREST_MIPMAP_NEAREST:
					return GL_NEAREST_MIPMAP_NEAREST;
				case pio::gfx::MIN_FILTER_LINEAR_MIPMAP_NEAREST:
					return GL_LINEAR_MIPMAP_NEAREST;
				case pio::gfx::MIN_FILTER_NEAREST_MIPMAP_LINEAR:
					return GL_NEAREST_MIPMAP_LINEAR;
				case pio::gfx::MIN_FILTER_LINEAR_MIPMAP_LINEAR:
					return GL_LINEAR_MIPMAP_LINEAR;
				case pio::gfx::MIN_FILTER_LINEAR:
				default:
					return GL_LINEAR;
			}
		}

		uint32_t Texture::getMagFilter(TextureFilterMag filter)
		{
			switch (filter)
			{
				case pio::gfx::MAG_FILTER_NEAREST:
					return GL_NEAREST;
				case pio::gfx::MAG_FILTER_LINEAR:
				default:
					return GL_LINEAR;
			}
		}

		uint32_t Texture::getWrap(TextureWrap wrap)
		{
			switch (wrap)
			{
				case pio::gfx::WRAP_REPEAT:
					return GL_REPEAT;
				case pio::gfx::WRAP_CLAMP_TO_BORDER:
					return GL_CLAMP_TO_BORDER;
				case pio::gfx::WRAP_MIRROED_REPEAT:
					return GL_MIRRORED_REPEAT;
				case pio::gfx::WRAP_MIRROR_CLAMP_TO_EDGE:
					return GL_MIRROR_CLAMP_TO_EDGE;
				case pio::gfx::WRAP_CLAMP_TO_EDGE:
				default:
					return GL_CLAMP_TO_EDGE;
			}
		}

		uint8_t *Texture::loadImgData()
		{
			uint8_t *data = stbi_load(m_srcPath.c_str(), &m_width, &m_height, &m_channel, 0);
			if (!data)
				LOG_ERR("fail to load image data from path[%s]", m_srcPath.c_str());
			return data;
		}

		void Texture::freeImgData(uint8_t *data)
		{
			stbi_image_free(data);
			data = nullptr;
		}

		template<>
		bool Buffer::is<Texture>()
		{
			return m_bufferType == GFX_BUF_TEXTURE;
		}

		template<>
		Texture *Buffer::as<Texture>()
		{
			if (is<Texture>())
			{
				return static_cast<Texture *>(this);
			}
			return nullptr;
		}
	}
}