#include "SpriteMaterial.h"

#include "gfx/renderer/RenderContext.h"

namespace pio
{
	SpriteMaterial::SpriteMaterial(const std::string& name, RenderingMode mode)
		: Material(name, mode, ShaderSpec_Sprite)
	{
	}

	SpriteMaterial::SpriteMaterial(const SpriteMaterial& rhs) : Material(rhs)
	{
		m_color = rhs.m_color;
		m_flipX = rhs.m_flipX;
		m_flipY = rhs.m_flipY;
		m_spriteTexture = rhs.m_spriteTexture;
	}

	SpriteMaterial& SpriteMaterial::operator=(const SpriteMaterial& rhs)
	{
		if(this != &rhs)
		{
			this->SpriteMaterial::SpriteMaterial(rhs);
		}
		return *this;
	}

	void SpriteMaterial::update(Ref<RenderContext>& context)
	{
		updateUnimData(GpuAttr::SpriteColor, m_color);
		updateUnimData(GpuAttr::SpriteFlipX, m_flipX);
		updateUnimData(GpuAttr::SpriteFlipY, m_flipY);
		updateUnimData(GpuAttr::SpriteOverlay, m_renderingMode == RenderingMode_Overlay);

		updateTexture(GpuAttr::SpriteTexture, m_spriteTexture.use_count() != 0 ? m_spriteTexture : context->getTexture(pio::GpuAttr::Tex::WHITE));
	}

	Ref<Asset> SpriteMaterial::clone() const
	{
		auto material = CreateRef<SpriteMaterial>(m_name, m_renderingMode);
		*(material.get()) = *this;		
		material->m_hnd = UUID32();
		return material;
	}

	template<>
	bool Asset::is<SpriteMaterial>() const
	{
		if(is<Material>())
		{
			return as<Material>()->spec() == ShaderSpec_Sprite;
		}
		return false;
	}
}