#pragma once

#include "Material.h"

namespace pio
{
	class Image;

	class TexturedMaterial : public Material
	{
		RTTR_ENABLE(Material)
	public:
		TexturedMaterial(const ImportParams& params);
		virtual ~TexturedMaterial() = default;		
		virtual bool BindAt(const Ref<Shader>& shader) override;
		virtual bool Bind() override;
		virtual bool UnBind() override;
		virtual Ref<Asset> SpawnChild() override;
		virtual bool IsTranslucency() const override;

		void SetTexture(const Ref<Image>& img) { m_Texture = img; }

	protected:
		virtual void OnInit() override;
		virtual void BuildUnformBuffer() override;

	private:
		PIO_DEFINE_PROPERTY(glm::vec3, Color, glm::vec3(1.f))
		PIO_DEFINE_PROPERTY(glm::vec2, Tile, glm::vec2(1.f))		
		PIO_DEFINE_PROPERTY(glm::vec2, Scale, glm::vec2(1.f))
		PIO_DEFINE_PROPERTY(glm::vec2, Offset, glm::vec2(0.f))
		PIO_DEFINE_PROPERTY(float, Alpha, 1.f)
		PIO_DEFINE_PROPERTY(bool, FlipY, false)

		Ref<Image> m_Texture;
	};

}