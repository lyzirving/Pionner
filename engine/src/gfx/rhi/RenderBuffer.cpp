#include "RenderBuffer.h"

#include "gfx/rhi/RenderAPI.h"
#include "platform/opengl/GLRenderBuffer.h"

namespace pio
{
	Ref<RenderBuffer> RenderBuffer::Create(const TextureSpecification &spec)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLRenderBuffer>(spec);
			default:
				return Ref<RenderBuffer>();
		}
	}

	template<>
	bool Texture::is<RenderBuffer>() const { return this->getAssetType() == AssetType::RenderBuffer; }
}