#include "MeshData.h"

#include "gfx/renderer/RenderContext.h"

#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

namespace pio
{
	MeshBuffer::MeshBuffer(const MeshBuffer& rhs) : Vao(rhs.Vao), Vbo(rhs.Vbo), Ebo(rhs.Ebo)
	{
	}

	MeshBuffer::MeshBuffer(MeshBuffer&& rhs) noexcept
	{
		Vao = std::move(rhs.Vao);
		Vbo = std::move(rhs.Vbo);
		Ebo = std::move(rhs.Ebo);
	}

	MeshBuffer& MeshBuffer::operator=(const MeshBuffer& rhs)
	{
		if (this != &rhs)
		{
			this->MeshBuffer::MeshBuffer(rhs);
		}
		return *this;
	}

	MeshBuffer& MeshBuffer::operator=(MeshBuffer&& rhs) noexcept
	{
		if (this != &rhs)
		{
			this->MeshBuffer::MeshBuffer(std::forward<MeshBuffer>(rhs));
		}
		return *this;
	}
}