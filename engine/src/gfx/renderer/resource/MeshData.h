#ifndef __PIONNER_GFX_RENDERER_RESOURCE_MESH_DATA_H__
#define __PIONNER_GFX_RENDERER_RESOURCE_MESH_DATA_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexArray;
	class RenderContext;

	struct MeshBuffer
	{
		Ref<VertexArray> Vao;
		Ref<VertexBuffer> Vbo;
		Ref<IndexBuffer> Ebo;

		MeshBuffer() {}

		MeshBuffer(const MeshBuffer& rhs);
		MeshBuffer(MeshBuffer&& rhs) noexcept;

		MeshBuffer& operator=(const MeshBuffer& rhs);
		MeshBuffer& operator=(MeshBuffer&& rhs) noexcept;

		bool valid() const { return Vao.use_count() != 0 && Vbo.use_count() != 0 && Ebo.use_count() != 0; }
	};
}

#endif