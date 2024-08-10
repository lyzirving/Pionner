#include "GLFrameBuffer.h"

#include "GLHelper.h"

#include "asset/AssetsManager.h"

#include "gfx/rhi/Texture.h"
#include "gfx/rhi/RenderBuffer.h"
#include "gfx/renderer/RenderData.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLFrameBuffer"

namespace pio
{
	GLFrameBuffer::GLFrameBuffer(const FrameBufferSpecification& spec) : FrameBuffer(), m_spec(spec)
	{
		uint32_t num = std::min(PIO_UINT(ColorAttachment::Num), PIO_UINT(m_spec.ColorBufferSpec.size()));
		m_colorBuffers.resize(num);

		for (uint32_t i = 0; i < num; i++)
		{
			auto& spec = m_spec.ColorBufferSpec[i];
			if (spec.Format == ImageInternalFormat::None)
				continue;

			switch (spec.AType)
			{
			case AssetType::Texture2D:
			{
				m_colorBuffers[i] = Texture2D::Create(spec);
				if (spec.AddToAssets)
				{
					AssetsManager::Get()->addRuntimeAsset(m_colorBuffers[i]);
				}
				break;
			}
			case AssetType::CubeTexture:
			{
				m_colorBuffers[i] = CubeTexture::Create(spec);
				if (spec.AddToAssets)
				{
					AssetsManager::Get()->addRuntimeAsset(m_colorBuffers[i]);
				}
				break;
			}
			default:
				LOGE("err! invalid AType[%u] for color buffer", spec.AType);
				break;
			}
		}

		if (m_spec.DepthBufferSpec.Format != ImageInternalFormat::None)
		{
			switch (m_spec.DepthBufferSpec.AType)
			{
			case AssetType::Texture2D:
				m_depthBuffer = Texture2D::Create(m_spec.DepthBufferSpec);
				break;
			case AssetType::CubeTexture:
				m_depthBuffer = CubeTexture::Create(m_spec.DepthBufferSpec);
				break;
			case AssetType::CubeArrayTexture:
				m_depthBuffer = CubeArrayTexture::Create(m_spec.DepthBufferSpec);
				break;
			case AssetType::RenderBuffer:
				m_depthBuffer = RenderBuffer::Create(m_spec.DepthBufferSpec);
				break;
			default:
				LOGE("fail to find valid assets type[%u]", m_spec.DepthBufferSpec.AType);
				break;
			}
			m_depthSlot = m_spec.DepthAttachment;
		}
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		destroy();
	}

	bool GLFrameBuffer::init()
	{
		if (!isInit())
		{
			for (auto& colorAttach : m_colorBuffers)
				colorAttach->init();

			if (m_depthBuffer) m_depthBuffer->init();

			glGenFramebuffers(1, &m_id);
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);

			if (FrameBuffer::IsColorBuffer(m_spec.FrameBufferUsage) && !m_colorBuffers.empty())
			{
				std::vector<uint32_t> colorAttachments;
				for (uint32_t i = 0; i < m_colorBuffers.size(); i++)
				{
					m_colorBuffers[i]->bind();
					if (m_colorBuffers[i]->is<Texture2D>())
					{
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorBuffers[i]->getId(), 0);
					}
					else if (m_colorBuffers[i]->is<CubeTexture>())
					{
						for (uint32_t j = 0; j < LightDir_Num; ++j)
						{
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
								GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, m_colorBuffers[i]->getId(), 0);
						}
					}
					colorAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);
				}
				glDrawBuffers(colorAttachments.size(), colorAttachments.data());

				if (m_depthBuffer)
				{
					m_depthBuffer->bind();
					if (m_depthBuffer->is<RenderBuffer>())
					{
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GLHelper::GetGLDepthAttachment(m_depthSlot),
							GL_RENDERBUFFER, m_depthBuffer->getId());
					}
				}
			}
			else if (FrameBuffer::IsDepthBuffer(m_spec.FrameBufferUsage) && m_depthBuffer)
			{
				m_depthBuffer->bind();
				if (m_depthBuffer->is<Texture2D>())
				{
					glFramebufferTexture2D(GL_FRAMEBUFFER, GLHelper::GetGLDepthAttachment(m_depthSlot),
						GL_TEXTURE_2D, m_depthBuffer->getId(), 0);
				}
				else if (m_depthBuffer->is<CubeTexture>() || m_depthBuffer->is<CubeArrayTexture>())
				{
					glFramebufferTexture(GL_FRAMEBUFFER, GLHelper::GetGLDepthAttachment(m_depthSlot),
						m_depthBuffer->getId(), 0);
				}
				else
				{
					LOGE("invalid depth buffer type");
				}
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
			}
			else if (FrameBuffer::IsDepthStencil(m_spec.FrameBufferUsage) && m_depthBuffer)
			{
				//TODO:
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
			for (auto& colorAttach : m_colorBuffers)
			{
				colorAttach->destroy();
				colorAttach.reset();
			}
			m_colorBuffers.clear();
			m_depthBuffer.reset();
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

	void GLFrameBuffer::bindTarget(ColorAttachment attach, LightDir face, uint32_t mipLevel)
	{
		init();

		if (isInit())
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + PIO_UINT(attach),
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
				m_colorBuffers[PIO_UINT(attach)]->getId(), mipLevel);
			GLHelper::CheckError("bindTarget: frame buffer[%s] fail!! attachment[%u], face[%u]",
				m_spec.Name.c_str(), PIO_UINT(attach), face);
		}
	}

	void GLFrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}