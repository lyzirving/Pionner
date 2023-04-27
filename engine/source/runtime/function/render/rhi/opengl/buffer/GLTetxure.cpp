#ifndef STB_IMAGE_STATIC
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_STATIC
#include <stb/stb_image.h>

#include "function/render/rhi/opengl/buffer/GLTetxure.h"
#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/RhiHeader.h"

#include "function/render/resource/RenderResourceMgr.h"

namespace Pionner
{
	GLTexture::GLTexture(const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxBuffer(mgr), m_path(), m_data(nullptr),
		m_width(0), m_height(0), m_channel(0)
	{
		m_dataType = DATA_TEXTURE;
		m_bufferType = BUF_TEXTURE;
	}

	GLTexture::~GLTexture()
	{
		GLTexture::release();
	}

	void GLTexture::upload()
	{
		if (isUpload())
			return;

		if (!m_data)
			return;

		if (m_width == 0 || m_height == 0 || m_channel == 0)
			return;

		if (!isCreated())
		{
			glGenTextures(1, &m_id);
		}

		GLenum fmt = colorFormat(m_channel);

		glBindTexture(GL_TEXTURE_2D, m_id);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, fmt, m_width, m_height, 0, fmt, GL_UNSIGNED_BYTE, m_data);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(m_data);
		m_data = nullptr;

		m_uploaded = true;
	}

	void GLTexture::release()
	{
		if (isCreated())
		{
			std::shared_ptr<RenderResourceMgr> mgr = m_mgr.lock();
			if (mgr)
			{
				mgr->release(m_bufferType, m_slot);
			}
		}
	}

	void GLTexture::bindToTarget(uint32_t target)
	{
		if (!isUpload())
		{
			upload();
		}

		if (isUpload())
		{
			glActiveTexture(GL_TEXTURE0 + target);
			glBindTexture(GL_TEXTURE_2D, m_id);
		}
	}

	void GLTexture::load()
	{
		if (m_path.empty())
			return;

		if (m_data)
			return;

		m_data = stbi_load(m_path.c_str(), &m_width, &m_height, &m_channel, 0);
	}

	uint32_t GLTexture::colorFormat(int32_t component)
	{
		if (component == 1)
			return GL_RED;
		else if (component == 2)
			return GL_RG;
		else if (component == 3)
			return GL_RGB;
		else
			return GL_RGBA;
	}

	template<>
	bool GfxBuffer::is<GLTexture>() const
	{
		return getDataType() == DATA_TEXTURE;
	}

	template<>
	void GfxBuffer::insertData<std::string>(std::string &data)
	{
		if (is<GLTexture>())
		{
			GLTexture *ptr = (GLTexture *)this;
			ptr->m_path = data;
		}
	}
}