#ifndef __PIONNER_GFX_RESOURCE_RENDER_TARGET_H__
#define __PIONNER_GFX_RESOURCE_RENDER_TARGET_H__

#include "gfx/GfxDef.h"
#include "asset/Asset.h"

namespace pio
{
	class FrameBuffer;
	class RenderContext;
	struct FrameBufferSpecific;

	class RenderTarget : public Asset
	{
		PIO_DEFINE_ASSET_TYPE(AssetType::RenderTarget)
	public:
		RenderTarget();
		~RenderTarget() = default;

		Ref<FrameBuffer>& frameBuffer() { return m_frameBuffer; }

	protected:
		Ref<FrameBuffer> m_frameBuffer;

	public:
		static Ref<RenderTarget> Create(Ref<RenderContext>& context, const FrameBufferSpecific& spec);
	};

	template<>
	bool Asset::is<RenderTarget>() const;
}

#endif