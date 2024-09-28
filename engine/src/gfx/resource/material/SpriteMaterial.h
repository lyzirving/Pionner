#ifndef __PIONNER_GFX_RESOURCES_MATERIAL_SPRITE_MATERIAL_H__
#define __PIONNER_GFX_RESOURCES_MATERIAL_SPRITE_MATERIAL_H__

#include "Material.h"

namespace pio
{
	class Texture;

	class SpriteMaterial : public Material
	{
	public:
		SpriteMaterial(const std::string& name, RenderingMode mode = RenderingMode_Overlay);
		SpriteMaterial(const SpriteMaterial& rhs);
		SpriteMaterial& operator=(const SpriteMaterial& rhs);
		~SpriteMaterial() = default;
		virtual void onUpdate(Ref<RenderContext>& context) override;

		void setColor(const glm::vec3& color) { m_color = color; }
		void setFlipX(bool flip) { m_flipX = flip; }
		void setFlipY(bool flip) { m_flipY = flip; }
		void setSpriteTexture(const Ref<Texture>& texture) { if(m_spriteTexture != texture) { m_spriteTexture = texture; } }

		virtual Ref<Asset> clone() const override;

	private:
		struct GpuAttr
		{
			static constexpr char* SpriteColor = "u_spriteMaterial.Color";
			static constexpr char* SpriteFlipX = "u_spriteMaterial.FlipX";
			static constexpr char* SpriteFlipY = "u_spriteMaterial.FlipY";
			static constexpr char* SpriteOverlay = "u_spriteMaterial.Overlay";
			static constexpr char* SpriteTexture = "u_spriteMaterial.SpriteTexture";
		};

	private:
		glm::vec3 m_color{ 1.f };
		bool m_flipX{ false }, m_flipY{ false };
		Ref<Texture> m_spriteTexture;
	};

	template<>
	bool Asset::is<SpriteMaterial>() const;
}

#endif