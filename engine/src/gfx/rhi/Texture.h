#pragma once

#include "RenderResource.h"

namespace pio
{
	struct TextureSpecific
	{
		std::string Name;
		TextureType Type{ TextureType::Num };
		TextureFormat Format{ TextureFormat::RGB_24 };
		int32_t Width{ 1 }, Height{ 1 }, Channel{ 1 }, Num{ 1 };
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

		const TextureSpecific& Build() { return m_Spec; }

		TextureSpecificBuilder& Name(const std::string& name) { m_Spec.Name = name; return *this; }
		TextureSpecificBuilder& Type(TextureType type) { m_Spec.Type = type; return *this; }
		TextureSpecificBuilder& Format(TextureFormat fmt) { m_Spec.Format = fmt; return *this; }
		TextureSpecificBuilder& Width(int32_t val) { m_Spec.Width = val; return *this; }
		TextureSpecificBuilder& Height(int32_t val) { m_Spec.Height = val; return *this; }
		TextureSpecificBuilder& Channel(int32_t val) { m_Spec.Channel = val; return *this; }
		TextureSpecificBuilder& TexWrap(TextureWrap s, TextureWrap t, TextureWrap r = TextureWrap::ClampEdge) { m_Spec.WrapS = s; m_Spec.WrapT = t; m_Spec.WrapR = r; return *this; }
		TextureSpecificBuilder& TexFilter(TextureFilterMin min, TextureFilterMag mag) { m_Spec.MinFilter = min; m_Spec.MaxFilter = mag; return *this; }
		TextureSpecificBuilder& Border(const glm::vec4& color) { m_Spec.BorderColor = color; return *this; }
		TextureSpecificBuilder& Mipmap(bool gen) { m_Spec.GenerateMips = gen; return *this; }
		TextureSpecificBuilder& FlipV(bool val) { m_Spec.FlipVerticalWhenLoad = val; return *this; }
		TextureSpecificBuilder& Num(int32_t val) { m_Spec.Num = val; return *this; }

	private:
		TextureSpecific m_Spec;
	};

	class RenderContext;
	class Buffer;
	class Shader;

	class Texture : public RenderResource
	{
		RTTR_ENABLE(RenderResource)
	public:
		Texture(const Ref<RenderContext>& context, const TextureSpecific& spec) : RenderResource(context, RenderResourceType::Texture, spec.Name), m_Spec(spec) {}
		Texture(const Ref<RenderContext>& context, const TextureSpecific& spec, RenderResourceType type) : RenderResource(context, type, spec.Name), m_Spec(spec) {}
		virtual ~Texture() = default;

		TextureSpecific& Spec() { return m_Spec; }
		const TextureSpecific& Spec() const { return m_Spec; }
		uint32_t Width() const { return Spec().Width; }
		uint32_t Height() const { return Spec().Height; }
		TextureType GetTextureType() const { return Spec().Type; }
		TextureFormat GetTextureFmt() const { return Spec().Format; }

	private:
		friend class TextureMgr;
		
		static Ref<Texture> Create(const Ref<RenderContext>& context, const TextureSpecific& spec);
		static Ref<Texture> Create(const Ref<RenderContext>& context, const TextureSpecific& spec, const std::string& path);
		static Ref<Texture> Create(const Ref<RenderContext>& context, const TextureSpecific& spec, Buffer& buffer);
		static Ref<Texture> Create(const Ref<RenderContext>& context, const TextureSpecific& spec, uint8_t** buffer);

	protected:
		TextureSpecific m_Spec;
	};

	class Texture2D : public Texture
	{
		RTTR_ENABLE(Texture)
	public:
		Texture2D(const Ref<RenderContext>& context, const TextureSpecific& spec);
		virtual ~Texture2D() = default;

		virtual void Active(TextureSampler sampler) = 0;
		/*
		* @brief bind texture at a specific slot in shader
		* @param shader: shader to be bound
		* @param name: slot's name in shader
		* @return true: succeed to bind. false: fail to bind.
		*/
		virtual bool BindAt(const Ref<Shader>& shader, const std::string& name) = 0;
		virtual void* Data() const = 0;
	};

	class CubeMap : public Texture2D
	{
		RTTR_ENABLE(Texture2D)
	public:
		CubeMap(const Ref<RenderContext>& context, const TextureSpecific& spec);
		virtual ~CubeMap() = default;		
	};

	class CubeMapArray : public CubeMap
	{
		RTTR_ENABLE(CubeMap)
	public:
		CubeMapArray(const Ref<RenderContext>& context, const TextureSpecific& spec);
		virtual ~CubeMapArray() = default;
	};

	class RenderBuffer : public Texture
	{
		RTTR_ENABLE(Texture)
	public:
		RenderBuffer(const Ref<RenderContext>& context, const TextureSpecific& spec);
		virtual ~RenderBuffer() = default;

		virtual void Bind(uint32_t width, uint32_t height) = 0;
	};
}