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

	void Mesh::update(Ref<RenderContext>& context, const TransformComponent& comp)
	{
		setUp(context);

		m_transform.setEuler(comp.Rotation);
		m_transform.setScale(comp.Scale);
		m_transform.setPosition(comp.Position);

		m_uniforms[GpuAttr::UNI_MODEL_MAT]->write(&m_transform.transformMat());
	}

	template<>
	bool Asset::is<Mesh>() const { return assetType() == AssetType::Mesh; }
}