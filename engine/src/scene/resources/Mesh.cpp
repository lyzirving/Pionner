#include "Mesh.h"

#include "scene/Components.h"

#include "gfx/rhi/UniformData.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Mesh"

namespace pio
{	
	void Mesh::setUp(Ref<RenderContext>& context)
	{
		if (!m_data.valid())
		{
			MeshData::Create(context, m_data, m_triMesh.Vertices, m_triMesh.Indices);

			auto transUnim = CreateRef<UniformMat4>(GpuAttr::UNI_MODEL_MAT);
			m_uniforms.insert({ transUnim->name(), transUnim });
		}
	}

	void Mesh::update(const TransformComponent& comp)
	{
		PIO_CHECK_RETURN(m_data.valid(), "err! mesh data has not been set up");
		m_transform.Euler = comp.Rotation;
		m_transform.Scale = comp.Scale;
		m_transform.Position = comp.Position;

		m_uniforms[GpuAttr::UNI_MODEL_MAT]->write(&m_transform.mat());
	}

	template<>
	bool Asset::is<Mesh>() const { return assetType() == AssetType::Mesh; }
}