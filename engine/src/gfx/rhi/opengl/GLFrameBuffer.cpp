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
	GLFrameBuffer::GLFrameBuffer(const Ref<RenderContext>& context, const FrameBufferSpecific& spec)
		: FrameBuffer(context, spec)
	{
		uint32_t colorNum = std::min(PIO_UINT(FrameBufferAttach::ColorAttachNum), PIO_UINT(m_Spec.ColorSpec.size()));
		m_ColorBuffs.reserve(colorNum);

		for (uint32_t i = 0; i < colorNum; i++)
		{
			const TextureSpecific& texSpec = m_Spec.ColorSpec[i];
			if (texSpec.Type == TextureType::Num)
				continue;
			m_ColorBuffs.push_back(context->CreateTexture(texSpec));
		}
		m_ColorBuffs.shrink_to_fit();

		uint32_t depthNum = m_Spec.DepthSpec.size();
		m_DepthBuffs.reserve(depthNum);
		for (size_t i = 0; i < depthNum; i++)
		{
			if (m_Spec.DepthSpec[i].Type == TextureType::TwoDimen ||
				m_Spec.DepthSpec[i].Type == TextureType::CubeMap ||
				m_Spec.DepthSpec[i].Type == TextureType::CubeMapArray ||
				m_Spec.DepthSpec[i].Type == TextureType::RenderBuffer)
			{
				m_DepthBuffs.push_back(context->CreateTexture(m_Spec.DepthSpec[i]));
			}
			else
			{
				LOGW("warning! depth attachment is not defined or type is invalid");
			}
		}
		m_DepthBuffs.shrink_to_fit();
	}

	bool GLFrameBuffer::Init()
	{
		if (!IsInit())
		{
			for (auto& colorAttach : m_ColorBuffs)
				colorAttach->Init();

			for (auto& depth : m_DepthBuffs)
				depth->Init();

			glGenFramebuffers(1, &m_ID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

			if (PIO_FBO_IS_COLOR_BUF(m_Spec.Usage) && !m_ColorBuffs.empty())
			{
				std::vector<uint32_t> colorAttachments;
				for (uint32_t i = 0; i < m_ColorBuffs.size(); i++)
				{
					m_ColorBuffs[i]->Bind();
					if(m_ColorBuffs[i]->Is<CubeMap>())
					{
						auto* cubeMap = m_ColorBuffs[i]->As<CubeMap>();
						for(size_t j = 0; j < CubeMapDir_Num; ++j)
						{
							cubeMap->AttachFrameBuffer(Self<FrameBuffer>(), i, j);
						}
					}
					else if (m_ColorBuffs[i]->Is<Texture2D>())
					{
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorBuffs[i]->Id(), 0);
					}					
					colorAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);
				}
				glDrawBuffers(colorAttachments.size(), colorAttachments.data());

				if (!m_DepthBuffs.empty())
				{					
					if (!m_DepthBuffs[m_DepSel]->Is<RenderBuffer>())//Ignore the other depth buffers
					{
						LOGE("err! depth buffer should be RenderBuffer for Color Buffer Usage");
						std::abort();
					}
					m_DepthBuffs[m_DepSel]->Bind();
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GLHelper::GetDepthAttachment(m_Spec.DepthSpec[m_DepSel].Format), GL_RENDERBUFFER, m_DepthBuffs[m_DepSel]->Id());
				}				
			}
			else if (PIO_FBO_IS_DEPTH_STENCIL(m_Spec.Usage) && !m_DepthBuffs.empty())
			{				
			}
			else if (PIO_FBO_IS_DEPTH_BUF(m_Spec.Usage) && !m_DepthBuffs.empty())
			{
				m_DepthBuffs[m_DepSel]->Bind();
				//As Texture2D is base class of CubeMapArray, so if the real pointer
				//is CubeMapArray, it can also be casted to Texture2D.
				//So we must put CubeMapArray first, then Texture2D, and so on.
				if(m_DepthBuffs[m_DepSel]->Is<CubeMapArray>())
				{
					glFramebufferTexture(GL_FRAMEBUFFER, GLHelper::GetDepthAttachment(m_Spec.DepthSpec[m_DepSel].Format),
										 m_DepthBuffs[m_DepSel]->Id(), 0);
				}
				else if (m_DepthBuffs[m_DepSel]->Is<Texture2D>())
				{
					glFramebufferTexture2D(GL_FRAMEBUFFER, GLHelper::GetDepthAttachment(m_Spec.DepthSpec[m_DepSel].Format),
										   GL_TEXTURE_2D, m_DepthBuffs[m_DepSel]->Id(), 0);
				}
				else if(m_DepthBuffs[m_DepSel]->Is<RenderBuffer>())
				{					
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GLHelper::GetDepthAttachment(m_Spec.DepthSpec[m_DepSel].Format), 
											  GL_RENDERBUFFER, m_DepthBuffs[m_DepSel]->Id());
				}
				else
				{
					LOGE("err! invalid depth buffer type");
				}
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
			}

			if (GLHelper::CheckFrameBufferStatus("fail to check frame buffer[%s][%u]'s status", m_Spec.Name.c_str(), m_ID))
				LOGD("succeed to init frame buffer[%s][%u]", m_Spec.Name.c_str(), m_ID);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		return IsInit();
	}

	void GLFrameBuffer::Destroy()
	{
		if (IsInit())
		{
			for (auto& colorAttach : m_ColorBuffs)
			{
				colorAttach->Destroy();
				colorAttach.reset();
			}
			m_ColorBuffs.clear();
			m_DepthBuffs.clear();
			glDeleteFramebuffers(1, &m_ID);
			m_ID = 0;
		}
	}

	void GLFrameBuffer::Bind()
	{
		Init();

		if (IsInit())
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
			m_Bound = GLHelper::CheckError("fail to bind fbo[%u][%s]", m_ID, m_Spec.Name.c_str());
		}
	}

	void GLFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_Bound = false;
	}

	void GLFrameBuffer::BindWritingDepth(uint8_t sel)
	{
		if (!PIO_FBO_IS_DEPTH_BUF(m_Spec.Usage))
		{
			LOGE("err! only available for depth buffer");
			return;
		}
		Init();
		if (IsInit())
		{
			SetDepBinding(sel);
			glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GLHelper::GetDepthAttachment(m_Spec.DepthSpec[m_DepSel].Format),
								   GL_TEXTURE_2D, m_DepthBuffs[m_DepSel]->Id(), 0);
			GLHelper::CheckError("fail to bind fbo[%u][%s]", m_ID, m_Spec.Name.c_str());
		}
	}

	bool GLFrameBuffer::SetDepBinding(uint8_t sel)
	{
		if (sel >= m_DepthBuffs.size())
		{
			LOGE("err! depth selection[%u] out of range[%u]", sel, m_DepthBuffs.size());
			return false;
		}
		m_DepSel = sel;
		return true;
	}
}