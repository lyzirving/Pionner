#include "Mesh.h"

#include "gfx/renderer/RenderContext.h"

#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Mesh"

namespace pio
{	
	void Mesh::setUp(Ref<RenderContext>& context)
	{
		if (!m_buffer.valid())
		{
		}
	}

	template<>
	bool Asset::is<Mesh>() const { return assetType() == AssetType::Mesh; }
}