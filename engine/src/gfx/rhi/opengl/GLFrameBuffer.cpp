#include "GLFrameBuffer.h"

#include "GLHelper.h"
#include "GLHeader.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/Texture.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLFrameBuffer"

namespace pio
{
	GLFrameBuffer::GLFrameBuffer(Ref<RenderContext>& context, const FrameBufferSpecific& spec)
		: FrameBuffer(context, spec)
	{
		uint32_t num = std::min(PIO_UINT(FrameBufferAttach::ColorAttachNum), PIO_UINT(m_spec.ColorSpec.size()));
		m_colorBuffs.reserve(num);

		for (uint32_t i = 0; i < num; i++)
		{
			const TextureSpecific& texSpec = m_spec.ColorSpec[i];
			if (texSpec.Type == TextureType::Num)
				continue;
			m_colorBuffs[i] = Texture::Create(context, texSpec);
		}
		m_colorBuffs.shrink_to_fit();

		if (m_spec.DepthSpec.Type == TextureType::TwoDimen  ||
			m_spec.DepthSpec.Type == TextureType::CubeMap   ||
			m_spec.DepthSpec.Type == TextureType::CubeArray ||
			m_spec.DepthSpec.Type == TextureType::RenderBuffer)
		{
			m_depthBuff = Texture::Create(context, m_spec.DepthSpec);
		}
		else
		{
			LOGE("err! depth attachment's texture type is not valid");
		}
	}

	bool GLFrameBuffer::init()
	{
		if (!isInit())
		{
			for (auto& colorAttach : m_colorBuffs)
				colorAttach->init();

			if (m_depthBuff) m_depthBuff->init();

			glGenFramebuffers(1, &m_id);
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);

			if (PIO_FBO_IS_COLOR_BUF(m_spec.Usage) && !m_colorBuffs.empty())
			{
				std::vector<uint32_t> colorAttachments;
				for (uint32_t i = 0; i < m_colorBuffs.size(); i++)
				{
					m_colorBuffs[i]->bind();
					if (m_colorBuffs[i]->is<Texture2D>())
					{
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorBuffs[i]->id(), 0);
					}
					colorAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);
				}
				glDrawBuffers(colorAttachments.size(), colorAttachments.data());

				if (m_depthBuff)
				{
				}
			}
			else if (PIO_FBO_IS_DEPTH_STENCIL(m_spec.Usage) && m_depthBuff)
			{

			}
			else if (PIO_FBO_IS_DEPTH_BUF(m_spec.Usage) && m_depthBuff)
			{

			}

			if (GLHelper::CheckFrameBufferStatus("fail to generate frame buffer[%s]", m_spec.Name.c_str()))
				LOGD("succeed to init frame buffer[%s][%u]", m_spec.Name.c_str(), m_id);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		return isInit();
	}

	void GLFrameBuffer::destroy()
	{
		if (isInit())
		{
			for (auto& colorAttach : m_colorBuffs)
			{
				colorAttach->destroy();
				colorAttach.reset();
			}
			m_colorBuffs.clear();
			m_depthBuff.reset();
			glDeleteFramebuffers(1, &m_id);
			m_id = 0;
		}
	}

	void GLFrameBuffer::bind()
	{
		init();

		if (isInit())
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
			GLHelper::CheckError("fail to bind fbo[%u][%s]", m_id, m_spec.Name.c_str());
		}
	}

	void GLFrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}