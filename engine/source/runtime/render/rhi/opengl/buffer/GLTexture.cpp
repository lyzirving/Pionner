#ifndef STB_IMAGE_STATIC
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_STATIC
#include <stb/stb_image.h>

#include "render/rhi/opengl/buffer/GLTexture.h"
#include "render/rhi/Rhi.h"
#include "render/rhi/RhiHeader.h"
#include "render/rhi/opengl/GLHelper.h"

#include "render/resource/RenderResourceMgr.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLTexture"

namespace pio
{
	GLTexture::GLTexture(const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxBuffer(mgr), m_path(), m_data(nullptr),
		m_width(0), m_height(0), m_channel(0)
	{
		m_bufferType = BUF_TEXTURE;
	}

	GLTexture::GLTexture(bool holder, const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxBuffer(holder, mgr), m_path(), m_data(nullptr),
		m_width(0), m_height(0), m_channel(0)
	{
		m_bufferType = BUF_TEXTURE;
	}

	GLTexture::~GLTexture() = default;

	void GLTexture::upload()
	{
		if (isAbandonded())
		{
			LOG_ERR("buf is already abandoned");
			return;
		}

		if (isUpload())
			return;

		if (!m_data)
		{
			LOG_ERR("data is null");
			return;
		}

		if (m_width == 0 || m_height == 0 || m_channel == 0)
			return;

		if (!isCreated())
		{
			glGenTextures(1, &m_id);
		}

		GLenum fmt = GLHelper::colorFormat(m_channel);

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

		m_uploaded = GLHelper::checkGLErr("fail to build texture");
	}

	void GLTexture::bindTarget(uint32_t target)
	{
		if (m_isHolder)
		{
			if (m_id != 0)
			{
				glActiveTexture(GL_TEXTURE0 + target);
				glBindTexture(GL_TEXTURE_2D, m_id);
			}
			else
			{
				LOG_ERR("holder texture, but id is invalid");
			}
			return;
		}

		if (isAbandonded())
		{
			LOG_ERR("buf is already abandoned");
			return;
		}

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

	void GLTexture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture::deleteResource()
	{
		if (isCreated())
		{
			glDeleteTextures(1, &m_id);
			m_id = 0;
		}
	}

	void GLTexture::loadRawData()
	{
		if (m_path.empty())
			return;

		if (m_data)
			return;

		m_data = stbi_load(m_path.c_str(), &m_width, &m_height, &m_channel, 0);
		if (!m_data)
			LOG_ERR("fail to load data from path[%s]", m_path.c_str());
	}

	template<>
	bool GfxBuffer::is<GLTexture>() const
	{
		return getBufferType() == BUF_TEXTURE;
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