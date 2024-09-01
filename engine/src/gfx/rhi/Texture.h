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

	class TextureSpecificBuilder
	{
	public:
		TextureSpecificBuilder() {}
		~TextureSpecificBuilder() = default;

		const TextureSpecific& build() { return m_spec; }

		TextureSpecificBuilder& name(const std::string& name) { m_spec.Name = name; return *this; }
		TextureSpecificBuilder& type(TextureType type) { m_spec.Type = type; return *this; }
		TextureSpecificBuilder& format(TextureFormat fmt) { m_spec.Format = fmt; return *this; }
		TextureSpecificBuilder& width(int32_t val) { m_spec.Width = val; return *this; }
		TextureSpecificBuilder& height(int32_t val) { m_spec.Height = val; return *this; }
		TextureSpecificBuilder& channel(int32_t val) { m_spec.Channel = val; return *this; }
		TextureSpecificBuilder& texWrap(TextureWrap s, TextureWrap t, TextureWrap r) { m_spec.WrapS = s; m_spec.WrapT = t; m_spec.WrapR = r; return *this; }
		TextureSpecificBuilder& texFilter(TextureFilterMin min, TextureFilterMag mag) { m_spec.MinFilter = min; m_spec.MaxFilter = mag; return *this; }
		TextureSpecificBuilder& border(const glm::vec4& color) { m_spec.BorderColor = color; return *this; }
		TextureSpecificBuilder& mipmap(bool gen) { m_spec.GenerateMips = gen; return *this; }
		TextureSpecificBuilder& flipV(bool val) { m_spec.FlipVerticalWhenLoad = val; return *this; }

	private:
		TextureSpecific m_spec;
	};

	class RenderContext;

	class Texture : public RenderResource
	{
		OVERRIDE_ASSET_TYPE(AssetType::Texture)
	public:
		Texture(Ref<RenderContext>& context, const TextureSpecific& spec) : RenderResource(context, RenderResourceType::Texture), m_spec(spec) {}
		Texture(Ref<RenderContext>& context, const TextureSpecific& spec, RenderResourceType type) : RenderResource(context, type), m_spec(spec) {}
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
		virtual ~Texture2D() = default;

		virtual void active(TextureSampler sampler) = 0;
	};

	class RenderBuffer : public Texture
	{
	public:
		RenderBuffer(Ref<RenderContext>& context, const TextureSpecific& spec);
		virtual ~RenderBuffer() = default;

		virtual void bind(uint32_t width, uint32_t height) = 0;
	};

	template<>
	bool Asset::is<Texture>() const;

	template<>
	bool Asset::is<Texture2D>() const;

	template<>
	bool Asset::is<RenderBuffer>() const;
}

#endif