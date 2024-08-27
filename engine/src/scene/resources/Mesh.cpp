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
			context->uploadData(m_data.Vbo);
			context->uploadData(m_data.Ebo);
			context->uploadData(m_data.Vao);

			auto transUnim = CreateRef<UniformMat4>("transform");
			m_uniforms.insert({ transUnim->name(), transUnim });
		}
	}

	void Mesh::update(const TransformComponent& comp)
	{
		if (!m_data.valid())
		{
			LOGE("err! mesh data has not been set up");
			return;
		}
		m_transform.Euler = comp.Rotation;
		m_transform.Scale = comp.Scale;
		m_transform.Position = comp.Position;

		m_uniforms["transform"]->write(&m_transform.mat());
	}

	template<>
	bool Asset::is<Mesh>() const { return assetType() == AssetType::Mesh; }
}