#ifndef __PIONNER_GFX_RHI_TEXTURE_H__
#define __PIONNER_GFX_RHI_TEXTURE_H__

#include "RenderResource.h"

namespace pio
{
	struct TextureSpecific
	{
		std::string Name;
		TextureType Type{ TextureType::Num };
		TextureFormat Format{ TextureFormat::RGB_24 };
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

	class RenderContext;

	class Texture : public RenderResource
	{
		OVERRIDE_ASSET_TYPE(AssetType::Texture)
	public:
		Texture(Ref<RenderContext>& context, const TextureSpecific& spec) : RenderResource(context, RenderResourceType::Texture), m_spec(spec) {}
		virtual ~Texture() = default;

		TextureSpecific& spec() { return m_spec; }
		const TextureSpecific& spec() const { return m_spec; }
		uint32_t width() const { return spec().Width; }
		uint32_t height() const { return spec().Height; }
		TextureType textureType() const { return spec().Type; }
		TextureFormat textureFmt() const { return spec().Format; }

	public:
		static Ref<Texture> Create(Ref<RenderContext>& context, const TextureSpecific& spec);

	protected:
		TextureSpecific m_spec;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(Ref<RenderContext>& context, const TextureSpecific& spec);
		~Texture2D() = default;

		virtual void active(TextureSampler sampler) = 0;
	};

	template<>
	bool Asset::is<Texture>() const;

	template<>
	bool Asset::is<Texture2D>() const;
}

#endif