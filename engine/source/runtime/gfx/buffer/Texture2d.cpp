#include "Texture2d.h"

#include "render/rhi/RhiHeader.h"
#include "gfx/context/GLHelper.h"

namespace pio
{
	namespace gfx
	{
		Texture2d::Texture2d() : Texture(TEXTURE_TYPE_2D)
		{	
		}

		Texture2d::Texture2d(SurfaceType type) : Texture(TEXTURE_TYPE_2D), m_surfaceType(type)
		{
		}

		Texture2d::~Texture2d()
		{
			Texture2d::release();
		}

		void Texture2d::upload()
		{
			if (isUpload() && !m_dataChange.load())
				return;

			if (isCreated())
				release();

			uint8_t *data = loadImgData();
			if (!data)
				return;

			glGenTextures(1, &m_id);

			GLenum fmt = gfx::GLHelper::colorFormat(m_channel);

			glBindTexture(GL_TEXTURE_2D, m_id);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture::getMinFilter(m_param.minFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Texture::getMagFilter(m_param.magFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Texture::getWrap(m_param.wrapS));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Texture::getWrap(m_param.wrapT));
			glTexImage2D(GL_TEXTURE_2D, 0, fmt, m_width, m_height, 0, fmt, GL_UNSIGNED_BYTE, data);

			glBindTexture(GL_TEXTURE_2D, 0);

			freeImgData(data);

			m_uploaded = gfx::GLHelper::checkGLErr("fail to build texture[%s]", m_srcPath.c_str());
			m_dataChange.store(false);
		}

		void Texture2d::bind()
		{
			bindTarget(0);
		}

		void Texture2d::bindTarget(uint32_t target)
		{
			if (!isUpload() || m_dataChange.load())
			{
				upload();
			}

			if (isUpload())
			{
				glActiveTexture(GL_TEXTURE0 + target);
				glBindTexture(GL_TEXTURE_2D, m_id);
			}
		}

		void Texture2d::unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		uint32_t Texture2d::size()
		{
			return m_width * m_height * m_channel;
		}

		void Texture2d::release()
		{
			if (isCreated())
			{
				glDeleteTextures(1, &m_id);
				m_id = 0;
				m_uploaded = false;
			}
		}

		template<>
		bool Buffer::is<Texture2d>()
		{
			auto *p = as<Texture>();
			if (p)
			{
				return p->getTexType() == TEXTURE_TYPE_2D;
			}
			return false;
		}

		template<>
		Texture2d *Buffer::as<Texture2d>()
		{
			if (is<Texture2d>())
			{
				return static_cast<Texture2d *>(this);
			}
			return nullptr;
		}
	}
}
