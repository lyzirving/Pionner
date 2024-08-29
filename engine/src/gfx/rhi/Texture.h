#ifndef __PIONNER_GFX_RHI_TEXTURE_H__
#define __PIONNER_GFX_RHI_TEXTURE_H__

#include "RenderResource.h"

namespace pio
{
	struct TextureSpecific
	{
		std::string Name;
		int32_t Width{ 1 }, Height{ 1 }, Channel{ 1 };
		TextureWrap WrapS{ TextureWrap::ClampEdge };
		TextureWrap WrapT{ TextureWrap::ClampEdge };
		TextureWrap WrapR{ TextureWrap::ClampEdge };
		TextureFilterMin MinFilter{ TextureFilterMin::Linear };
		TextureFilterMag MaxFilter{ TextureFilterMag::Linear };
		glm::vec4 BorderColor{ 1.f };

		bool GenerateMips{ false };
		bool FlipVerticalWhenLoad{ false };
	};

	class Texture : public RenderResource
	{
		OVERRIDE_ASSET_TYPE(AssetType::Texture)
	public:
		Texture(Ref<RenderContext>& context, const TextureSpecific& spec) : RenderResource(context, RenderResourceType::Texture), m_spec(spec) {}
		virtual ~Texture() = default;

		uint32_t width() const { return m_spec.Width; }
		uint32_t height() const { return m_spec.Height; }

	protected:
		TextureSpecific m_spec;
	};

	template<>
	bool Asset::is<Texture>() const;
}

#endif