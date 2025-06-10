#include "TexturedMaterial.h"
#include "asset/image/Image.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/Texture.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "TexturedMaterial"

namespace pio
{
	TexturedMaterial::TexturedMaterial(const ImportParams& params) : Material(params, SHADER_SPEC_TEXTURED)
	{
	}	

	bool TexturedMaterial::Bind(const Ref<Shader>& shader)
	{
		if(!Material::Bind(shader))
			return false;

		if(Texture2D* t; m_Texture && (t = m_Texture->GetTexture()->As<Texture2D>()))
		{
			t->BindAt(shader, "u_Texture");
		}

		return true;
	}

	bool TexturedMaterial::UnBind()
	{
		Material::UnBind();

		if(m_Texture)
		{
			m_Texture->GetTexture()->UnBind();
		}		
		return true;
	}

	Ref<Asset> TexturedMaterial::SpawnChild()
	{
		auto material = CreateRef<TexturedMaterial>(m_Params);
		material->m_Color = m_Color;
		material->m_Tile = m_Tile;
		material->m_Scale = m_Scale;
		material->m_Offset = m_Offset;
		material->m_Alpha = m_Alpha;

		material->m_Texture = m_Texture;
		material->Init();
		return material;
	}

	bool TexturedMaterial::IsTranslucency() const
	{
		return (!Math::Equal(m_Alpha, 1.f)) || (m_Texture && m_Texture->GetChannelNum() == 4);
	}

	void TexturedMaterial::OnInit()
	{
		OnColorChange();
		OnTileChange();
		OnScaleChange();
		OnOffsetChange();
		OnAlphaChange();
		OnFlipYChange();
	}

	void TexturedMaterial::BuildUnformBuffer()
	{
		if(m_UniformBlock)
			return;

		m_UniformBlock = CreateRef<UniformBlock>();
		m_UniformBlock->PushBack("Color", UniformBlock::CreateData(UniformType::Vec3, "Color"));
		m_UniformBlock->PushBack("Tile", UniformBlock::CreateData(UniformType::Vec2, "Tile"));
		m_UniformBlock->PushBack("Scale", UniformBlock::CreateData(UniformType::Vec2, "Scale"));
		m_UniformBlock->PushBack("Offset", UniformBlock::CreateData(UniformType::Vec2, "Offset"));		
		m_UniformBlock->PushBack("Alpha", UniformBlock::CreateData(UniformType::Float, "Alpha"));
		m_UniformBlock->PushBack("FlipY", UniformBlock::CreateData(UniformType::Bool, "FlipY"));
		m_UniformBlock->Calculate();

		m_UniformBuffer = UniformBuffer::Create(m_Context.lock(), m_UniformBlock->GetByteUsed(),
												UBBinding_TexturedMaterial, BufferUsage::Dynamic);
	}

	void TexturedMaterial::OnColorChange()
	{
		auto layout = (*m_UniformBlock)["Color"];
		m_UniformBlock->WriteAt(glm::value_ptr(m_Color), sizeof(glm::vec3), layout->GetAlignOffset());
	}

	void TexturedMaterial::OnTileChange()
	{
		auto layout = (*m_UniformBlock)["Tile"];
		m_UniformBlock->WriteAt(glm::value_ptr(m_Tile), sizeof(glm::vec2), layout->GetAlignOffset());
	}

	void TexturedMaterial::OnScaleChange()
	{
		auto layout = (*m_UniformBlock)["Scale"];
		m_UniformBlock->WriteAt(glm::value_ptr(m_Scale), sizeof(glm::vec2), layout->GetAlignOffset());
	}

	void TexturedMaterial::OnOffsetChange()
	{
		auto layout = (*m_UniformBlock)["Offset"];
		m_UniformBlock->WriteAt(glm::value_ptr(m_Offset), sizeof(glm::vec2), layout->GetAlignOffset());
	}	

	void TexturedMaterial::OnAlphaChange()
	{
		auto layout = (*m_UniformBlock)["Alpha"];
		m_UniformBlock->WriteAt(&m_Alpha, sizeof(float), layout->GetAlignOffset());
	}

	void TexturedMaterial::OnFlipYChange()
	{
		auto layout = (*m_UniformBlock)["FlipY"];
		m_UniformBlock->WriteAt(&m_FlipY, sizeof(bool), layout->GetAlignOffset());
	}
}