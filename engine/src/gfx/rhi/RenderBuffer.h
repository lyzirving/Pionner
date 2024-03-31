#ifndef __PIONNER_GFX_RHI_RENDER_BUFFER_H__
#define __PIONNER_GFX_RHI_RENDER_BUFFER_H__

#include "Texture.h"

namespace pio
{
	class RenderBuffer : public Texture
	{
		OVERRIDE_ASSET_TYPE(AssetType::RenderBuffer)
	public:
		RenderBuffer() : Texture() {}
		virtual ~RenderBuffer() = default;

		virtual void active(uint32_t sampler = 0) override { /* Not implemented*/ }
		virtual uint32_t getMipLevelCount() const override { return 0;/* Not implemented*/ }
		virtual void bind() override {};
		virtual void bind(uint32_t width, uint32_t height) = 0;

	public:
		static Ref<RenderBuffer> Create(const TextureSpecification &spec);
	};

	template<>
	bool Texture::is<RenderBuffer>() const;
}

#endif