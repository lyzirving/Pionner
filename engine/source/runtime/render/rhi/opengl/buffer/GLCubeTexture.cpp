#ifndef STB_IMAGE_STATIC
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_STATIC
#include <stb/stb_image.h>

#include "GLCubeTexture.h"

#include "render/rhi/Rhi.h"
#include "render/rhi/RhiHeader.h"
#include "gfx/context/GLHelper.h"

#include "render/resource/RenderResourceMgr.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLCubeTexture"

namespace pio
{
	GLCubeTexture::GLCubeTexture(const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxBuffer(mgr), m_info()
	{
		m_bufferType = BUF_CUBE_TEXTURE;
	}

	GLCubeTexture::GLCubeTexture(bool holder, const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxBuffer(holder, mgr), m_info()
	{
		m_bufferType = BUF_CUBE_TEXTURE;
	}

	GLCubeTexture::~GLCubeTexture() = default;

	void GLCubeTexture::upload()
	{
		if (isAbandonded())
		{
			LOG_ERR("buf is already abandoned");
			return;
		}

		if (isUpload())
			return;

		if (!isCreated())
		{
			glGenTextures(1, &m_id);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

		for (uint32_t i = 0; i < 6; i++)
		{
			if (m_info[i].data)
			{
				GLenum fmt = gfx::GLHelper::colorFormat(m_info[i].channel);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, fmt,
							 m_info[i].width, m_info[i].height, 0, fmt, GL_UNSIGNED_BYTE, 
							 m_info[i].data);
			}
			else
			{
				LOG_ERR("empty data for face[%u]", i);
			}
			stbi_image_free(m_info[i].data);
			m_info[i].data = nullptr;
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_uploaded = gfx::GLHelper::checkGLErr("fail to build texture");
	}

	void GLCubeTexture::bindTarget(uint32_t target)
	{
		if (m_isHolder)
		{
			if (m_id != 0)
			{
				glActiveTexture(GL_TEXTURE0 + target);
				glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
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
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
		}
	}

	void GLCubeTexture::unbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void GLCubeTexture::deleteResource()
	{
		if (isCreated())
		{
			glDeleteTextures(1, &m_id);
			m_id = 0;
		}
	}

	void GLCubeTexture::loadRawData()
	{
		if (isCreated())
			return;

		for (size_t i = 0; i < 6; i++)
		{
			if (m_info[i].path.empty())
			{
				LOG_ERR("data path[%u] is empty, break", i);
				continue;
			}
			if (m_info[i].data)
			{
				stbi_image_free(m_info[i].data);
			}
			m_info[i].data = stbi_load(m_info[i].path.c_str(), &m_info[i].width, &m_info[i].height, &m_info[i].channel, 0);
			if (!m_info[i].data)
			{
				LOG_ERR("fail to load data[%u] from path[%s]", i, m_info[i].path.c_str());
			}
		}
	}

	template<>
	bool GfxBuffer::is<GLCubeTexture>() const
	{
		return getBufferType() == BUF_CUBE_TEXTURE;
	}

	template<>
	void GfxBuffer::insertData<std::string>(std::vector<std::string> &data)
	{
		if (data.empty() || data.size() < 6)
			return;

		if (is<GLCubeTexture>())
		{
			GLCubeTexture *ptr = (GLCubeTexture *)this;
			for (size_t i = 0; i < 6; i++)
			{
				ptr->m_info[i].path = data[i];
			}
		}
	}
}