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
		uint32_t colorNum = std::min(PIO_UINT(FrameBufferAttach::ColorAttachNum), PIO_UINT(m_spec.ColorSpec.size()));
		m_colorBuffs.reserve(colorNum);

		for (uint32_t i = 0; i < colorNum; i++)
		{
			const TextureSpecific& texSpec = m_spec.ColorSpec[i];
			if (texSpec.Type == TextureType::Num)
				continue;
			m_colorBuffs.push_back(context->createTexture(texSpec));
		}
		m_colorBuffs.shrink_to_fit();

		uint32_t depthNum = m_spec.DepthSpec.size();
		m_depthBuffs.reserve(depthNum);
		for (size_t i = 0; i < depthNum; i++)
		{
			if (m_spec.DepthSpec[i].Type == TextureType::TwoDimen ||
				m_spec.DepthSpec[i].Type == TextureType::CubeMap ||
				m_spec.DepthSpec[i].Type == TextureType::CubeArray ||
				m_spec.DepthSpec[i].Type == TextureType::RenderBuffer)
			{
				m_depthBuffs.push_back(context->createTexture(m_spec.DepthSpec[i]));
			}
			else
			{
				LOGW("warning! depth attachment is not defined or type is invalid");
			}
		}
		m_depthBuffs.shrink_to_fit();
	}

	bool GLFrameBuffer::init()
	{
		if (!isInit())
		{
			for (auto& colorAttach : m_colorBuffs)
				colorAttach->init();

			for (auto& depth : m_depthBuffs)
				depth->init();

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

				if (!m_depthBuffs.empty())
				{					
					if (!m_depthBuffs[m_depSel]->is<RenderBuffer>())//Ignore the other depth buffers
					{
						LOGE("err! depth buffer should be RenderBuffer for Color Buffer Usage");
						std::abort();
					}
					m_depthBuffs[m_depSel]->bind();
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GLHelper::GetDepthAttachment(m_spec.DepthSpec[m_depSel].Format), GL_RENDERBUFFER, m_depthBuffs[m_depSel]->id());
				}				
			}
			else if (PIO_FBO_IS_DEPTH_STENCIL(m_spec.Usage) && !m_depthBuffs.empty())
			{				
			}
			else if (PIO_FBO_IS_DEPTH_BUF(m_spec.Usage) && !m_depthBuffs.empty())
			{
				m_depthBuffs[m_depSel]->bind();
				if (m_depthBuffs[m_depSel]->is<Texture2D>())
				{
					glFramebufferTexture2D(GL_FRAMEBUFFER, GLHelper::GetDepthAttachment(m_spec.DepthSpec[m_depSel].Format),
										   GL_TEXTURE_2D, m_depthBuffs[m_depSel]->id(), 0);
				}
				else
				{
					LOGE("err! invalid depth buffer type");
				}
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
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
			m_depthBuffs.clear();
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

	void GLFrameBuffer::bindWritingDepth(uint8_t sel)
	{
		if (!PIO_FBO_IS_DEPTH_BUF(m_spec.Usage))
		{
			LOGE("err! only available for depth buffer");
			return;
		}
		init();
		if (isInit())
		{
			setDepBinding(sel);
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GLHelper::GetDepthAttachment(m_spec.DepthSpec[m_depSel].Format),
								   GL_TEXTURE_2D, m_depthBuffs[m_depSel]->id(), 0);
			GLHelper::CheckError("fail to bind fbo[%u][%s]", m_id, m_spec.Name.c_str());
		}
	}

	bool GLFrameBuffer::setDepBinding(uint8_t sel)
	{
		if (sel >= m_depthBuffs.size())
		{
			LOGE("err! depth selection[%u] out of range[%u]", sel, m_depthBuffs.size());
			return false;
		}
		m_depSel = sel;
		return true;
	}
}