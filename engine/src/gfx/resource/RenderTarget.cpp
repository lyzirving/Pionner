#include "RenderTarget.h"

#include "asset/AssetMgr.h"

#include "gfx/rhi/FrameBuffer.h"
#include "gfx/renderer/RenderContext.h"

namespace pio
{
	RenderTarget::RenderTarget() : Asset()
	{
	}

	Ref<RenderTarget> RenderTarget::Create(Ref<RenderContext>& context, const FrameBufferSpecific& spec)
	{
		auto target = AssetMgr::MakeRuntimeAsset<RenderTarget>();
		target->m_frameBuffer = FrameBuffer::Create(context, spec);
		return target;
	}

	template<>
	bool Asset::is<RenderTarget>() const { return assetType() == AssetType::RenderTarget; }
}