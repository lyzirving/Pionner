#include "MeshRenderBuffer.h"

#include "gfx/rhi/Shader.h"

namespace pio
{
	MeshRenderBuffer::MeshRenderBuffer() : Object()
	{
	}

	MeshRenderBuffer::MeshRenderBuffer(const MeshRenderBuffer& rhs)
	{
		if (this != &rhs)
		{
			Vao = rhs.Vao;
			Vbo = rhs.Vbo;
			Ebo = rhs.Ebo;
		}
	}

	MeshRenderBuffer::MeshRenderBuffer(MeshRenderBuffer&& rhs) noexcept
	{		
		if (this != &rhs)
		{
			Vao = std::move(rhs.Vao);
			Vbo = std::move(rhs.Vbo);
			Ebo = std::move(rhs.Ebo);
		}
	}

	MeshRenderBuffer& MeshRenderBuffer::operator=(const MeshRenderBuffer& rhs)
	{
		if (this != &rhs)
		{
			this->MeshRenderBuffer::MeshRenderBuffer(rhs);
		}
		return *this;
	}

	MeshRenderBuffer& MeshRenderBuffer::operator=(MeshRenderBuffer&& rhs) noexcept
	{
		if (this != &rhs)
		{
			this->MeshRenderBuffer::MeshRenderBuffer(std::move(rhs));
		}
		return *this;
	}
}