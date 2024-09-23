#include "MeshRenderBuffer.h"

#include "gfx/rhi/UniformData.h"
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

	void MeshRenderBuffer::update(Ref<RenderContext>& context)
	{
		Uniforms[GpuAttr::UNI_MODEL_MAT]->write(&(Transform.transformMat()));
	}

	void MeshRenderBuffer::bind(Ref<Shader>& shader)
	{
		for (auto it : Uniforms)
		{
			shader->setUniformData(it.second);
		}
	}

	void MeshRenderBuffer::setUp(Ref<RenderContext>& context, Ref<Mesh>& mesh)
	{
		if (valid())
			return;

		const auto& tris = mesh->triangles();
		MeshRenderBuffer::Create(context, *this, tris.Vertices, tris.Indices);

		auto transUnim = CreateRef<UniformMat4>(GpuAttr::UNI_MODEL_MAT);
		Uniforms.insert({ transUnim->name(), transUnim });
	}

	template<>
	bool Asset::is<MeshRenderBuffer>() const { return assetType() == AssetType::MeshRenderBuffer; }
}