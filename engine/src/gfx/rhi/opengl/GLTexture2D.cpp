#include "GLTexture2D.h"

#include "gfx/rhi/opengl/GLHelper.h"
#include "gfx/rhi/opengl/GLHeader.h"

#include "gfx/rhi/Shader.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLTexture2D"

namespace pio
{
	GLTexture2D::GLTexture2D(const Ref<RenderContext>& context, const TextureSpecific& spec) : Texture2D(context, spec)
	{
	}	

	GLTexture2D::GLTexture2D(const Ref<RenderContext>& context, const TextureSpecific& spec, Buffer& buff) : Texture2D(context, spec)
	{
		buff.move(&m_Data);
	}

	GLTexture2D::GLTexture2D(const Ref<RenderContext>& context, const TextureSpecific& spec, void** buff) : Texture2D(context, spec)
	{
		m_Data = *buff;
		*buff = nullptr;
	}

	bool GLTexture2D::Init()
	{
		if (!IsInit())
		{
			glGenTextures(1, &m_ID);

			glBindTexture(GL_TEXTURE_2D, m_ID);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLHelper::GetTextureMinFilter(m_Spec.MinFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLHelper::GetTextureMagFilter(m_Spec.MaxFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLHelper::GetTextureWrap(m_Spec.WrapS));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLHelper::GetTextureWrap(m_Spec.WrapT));
			glTexImage2D(GL_TEXTURE_2D, 0,
						 GLHelper::GetTextureInternalFmt(m_Spec.Format),
						 m_Spec.Width, m_Spec.Height, 0,						 
						 GLHelper::GetTexturePixelFmt(m_Spec.Format), //specify data' format from cpu
						 GLHelper::GetTexturePixelType(m_Spec.Format),//specify data' type from cpu
						 m_Data);

			float borderColor[] = { m_Spec.BorderColor.x, m_Spec.BorderColor.y, m_Spec.BorderColor.z, m_Spec.BorderColor.w };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			if (m_Spec.GenerateMips)
				glGenerateMipmap(GL_TEXTURE_2D);

			if (GLHelper::CheckError("err! fail to init texture2D[%s]", m_Spec.Name.c_str()))
				LOGD("succeed to init texture2D[%s][%u]", m_Spec.Name.c_str(), m_ID);

			glBindTexture(GL_TEXTURE_2D, 0);

			if (m_Data)
			{
				std::free(m_Data);
				m_Data = nullptr;
			}
		}
		return IsInit();
	}

	void GLTexture2D::Destroy()
	{
		if (IsInit())
		{
			glDeleteTextures(1, &m_ID);
			m_ID = 0;
		}
	}

	void GLTexture2D::Bind()
	{
		Init();

		if (IsInit())
		{
			glBindTexture(GL_TEXTURE_2D, m_ID);
			m_Bound = GLHelper::CheckError("fail to bind texture[%s][%u]", m_Spec.Name.c_str(), m_ID);
		}
		else
		{
			LOGE("err! texture[%s] has not been initilized", m_Spec.Name.c_str());
		}
	}

	bool GLTexture2D::BindAt(const Ref<Shader>& shader, const std::string& name)
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
			return GLHelper::CheckError("fail to bind texture[%s][%u] at [%s]", m_Spec.Name.c_str(), m_ID, name.c_str());
		}
		return false;
	}

	void GLTexture2D::UnBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		m_Bound = false;
	}

	void GLTexture2D::Active(TextureSampler sampler)
	{
		if (sampler == TextureSampler::MaxSlotNum ||
			sampler == TextureSampler::InvalidSlot)
		{
			LOGE("err! invalid sampler[%u]", sampler);
			return;
		}

		Init();

		if (IsInit())
		{
			glActiveTexture(GL_TEXTURE0 + PIO_UINT8(sampler));
		}
		else
		{
			LOGE("err! texture[%s] has not been initilized", m_Spec.Name.c_str());
		}
	}
}