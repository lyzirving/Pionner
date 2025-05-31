#include "GLCubeMapArray.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/opengl/GLHelper.h"
#include "gfx/rhi/opengl/GLHeader.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLCubeMapArray"

namespace pio
{
	GLCubeMapArray::GLCubeMapArray(const Ref<RenderContext>& context, const TextureSpecific& spec)
		: CubeMapArray(context, spec)
	{
	}

	bool GLCubeMapArray::Init()
	{
		if(!IsInit())
		{
			glGenTextures(1, &m_ID);
			
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_ID);

			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GLHelper::GetTextureMinFilter(m_Spec.MinFilter));
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GLHelper::GetTextureMagFilter(m_Spec.MaxFilter));
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GLHelper::GetTextureWrap(m_Spec.WrapS));
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GLHelper::GetTextureWrap(m_Spec.WrapT));
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GLHelper::GetTextureWrap(m_Spec.WrapR));			
			
			//[Note] aspect of CubeMap(width / height) should be 1 : 1
			glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0,
						 GLHelper::GetTextureInternalFmt(m_Spec.Format),
						 m_Spec.Width, m_Spec.Height, m_Spec.Num * 6, 0, 
						 GLHelper::GetTexturePixelFmt(m_Spec.Format),
						 GLHelper::GetTexturePixelType(m_Spec.Format),
						 nullptr);

			if(GLHelper::CheckError("err happens when init cube map array[%s]", m_Spec.Name.c_str()))
				LOGD("succeed to init cube map array[%s][%u]", m_Spec.Name.c_str(), m_ID);

			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
		}
		return IsInit();
	}

	void GLCubeMapArray::Destroy()
	{
		if(IsInit())
		{
			glDeleteTextures(1, &m_ID);
			m_ID = 0;
		}
	}

	void GLCubeMapArray::Bind()
	{
		Init();

		if(IsInit())
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_ID);
			m_Bound = GLHelper::CheckError("fail to bind cube map array[%s][%u]", m_Spec.Name.c_str(), m_ID);
		}
		else
		{
			LOGE("cube map array[%s], invalid state[%u]", m_Spec.Name.c_str(), m_ID);
		}
	}

	bool GLCubeMapArray::BindAt(const Ref<Shader>& shader, const std::string& name)
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
			return GLHelper::CheckError("fail to bind CubeMapArray[%s][%u] at [%s]", m_Spec.Name.c_str(), m_ID, name.c_str());
		}
		return false;
	}

	void GLCubeMapArray::UnBind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
		m_Bound = false;
	}

	void GLCubeMapArray::Active(TextureSampler sampler)
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
			LOGE("cube map array[%s], invalid state[%u]", m_Spec.Name.c_str(), m_ID);
		}
	}
}