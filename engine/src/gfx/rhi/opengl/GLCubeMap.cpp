#include "GLCubeMap.h"

#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/Shader.h"
#include "gfx/rhi/opengl/GLHelper.h"
#include "gfx/rhi/opengl/GLHeader.h"

#ifndef STB_IMAGE_STATIC
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_STATIC
#include <stb/stb_image.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLCubeMap"

namespace pio
{
	GLCubeMap::GLCubeMap(const Ref<RenderContext>& context, const TextureSpecific& spec)
		: CubeMap(context, spec)
	{
	}

	bool GLCubeMap::Init()
	{
		if(!IsInit())
		{
			glGenTextures(1, &m_ID);

			glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

			// NOTE: Cube Texture's width should be equal to its height
			for(uint8_t i = 0; i < CubeMapDir_Num; ++i)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
							 GLHelper::GetTextureInternalFmt(m_Spec.Format),
							 m_Spec.Width, m_Spec.Height, 0,
							 GLHelper::GetTexturePixelFmt(m_Spec.Format),
							 GLHelper::GetTexturePixelType(m_Spec.Format),
							 nullptr);
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GLHelper::GetTextureMinFilter(m_Spec.MinFilter));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GLHelper::GetTextureMagFilter(m_Spec.MaxFilter));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GLHelper::GetTextureWrap(m_Spec.WrapS));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GLHelper::GetTextureWrap(m_Spec.WrapT));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GLHelper::GetTextureWrap(m_Spec.WrapR));

			if(m_Spec.GenerateMips)
				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			if(GLHelper::CheckError("err happens when init cube map[%s]", m_Spec.Name.c_str()))
				LOGD("succeed to init cube map[%s][%u], size[%u, %u]", m_Spec.Name.c_str(), m_ID, m_Spec.Width, m_Spec.Height);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
		return IsInit();
	}

	void GLCubeMap::Destroy()
	{
		if(IsInit())
		{
			glDeleteTextures(1, &m_ID);
			m_ID = 0;
		}
	}

	void GLCubeMap::Bind()
	{
		Init();

		if(IsInit())
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
			m_Bound = GLHelper::CheckError("fail to bind cube map[%s][%u]", m_Spec.Name.c_str(), m_ID);
		}
		else
		{
			LOGE("cube map[%s] invalid state[%u]", m_Spec.Name.c_str(), m_ID);
		}
	}

	bool GLCubeMap::BindAt(const Ref<Shader>& shader, const std::string& name)
	{
		if(!shader)
		{
			LOGE("shader is invalid for texture[%s][%u]", m_Spec.Name.c_str(), m_ID);
			return false;
		}
		TextureSampler slot{ TextureSampler::InvalidSlot };
		if(shader->GetSampler(slot))
		{
			Active(slot);
			Bind();
			shader->SetTextureSampler(name, slot);
			return GLHelper::CheckError("fail to bind CubeMap[%s][%u] at [%s]", m_Spec.Name.c_str(), m_ID, name.c_str());
		}
		return false;
	}

	void GLCubeMap::UnBind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		m_Bound = false;
	}

	void GLCubeMap::Active(TextureSampler sampler)
	{
		if(sampler == TextureSampler::MaxSlotNum ||
		   sampler == TextureSampler::InvalidSlot)
		{
			LOGE("err! invalid sampler[%u]", sampler);
			return;
		}

		Init();

		if(IsInit())
		{
			glActiveTexture(GL_TEXTURE0 + PIO_UINT8(sampler));
		}
		else
		{
			LOGE("cube map[%s] invalid state[%u]", m_Spec.Name.c_str(), m_ID);
		}
	}

	bool GLCubeMap::AttachFrameBuffer(const Ref<FrameBuffer>& fbo, uint32_t colorAttachment, uint32_t texTarget)
	{
		if(!fbo || !fbo->IsInit())
		{
			LOGE("err! frame buffer is invalid");
			return false;
		}
		Init();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachment, GL_TEXTURE_CUBE_MAP_POSITIVE_X + texTarget, Id(), 0);
		return GLHelper::CheckError("fail to attach frame buffer[%s] for cube map[%s][%u]", fbo->Name().c_str(), Name().c_str(), Id());
	}
}