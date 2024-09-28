#include "MeshRenderBuffer.h"

#include "gfx/rhi/Shader.h"
#include "gfx/resource/Mesh.h"

#include "scene/Components.h"

namespace pio
{
	MeshRenderBuffer::MeshRenderBuffer() : Asset()
	{
	}

	MeshRenderBuffer::MeshRenderBuffer(const MeshRenderBuffer& rhs)
	{
		if (this != &rhs)
		{
			Vao = rhs.Vao;
			Vbo = rhs.Vbo;
			Ebo = rhs.Ebo;
			Uniforms = rhs.Uniforms;
		}
	}

	MeshRenderBuffer::MeshRenderBuffer(MeshRenderBuffer&& rhs) noexcept
	{		
		if (this != &rhs)
		{
			Vao = std::move(rhs.Vao);
			Vbo = std::move(rhs.Vbo);
			Ebo = std::move(rhs.Ebo);
			Uniforms = std::move(rhs.Uniforms);
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
			this->MeshRenderBuffer::MeshRenderBuffer(std::forward<MeshRenderBuffer>(rhs));
		}
		return *this;
	}

	void MeshRenderBuffer::bind(Ref<Shader>& shader)
	{
		for (auto it : Uniforms)
		{
			shader->setUniformData(it.second);
		}
	}

	void MeshRenderBuffer::onUpdate(Ref<RenderContext>& context)
	{
		Uniforms[GpuAttr::UNI_MODEL_MAT]->write(&(Transform.transformMat()));
	}	

	template<>
	bool Asset::is<MeshRenderBuffer>() const { return assetType() == AssetType::MeshRenderBuffer; }
}