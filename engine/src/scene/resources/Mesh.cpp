#include "Mesh.h"

#include "scene/Components.h"

#include "gfx/rhi/UniformData.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Mesh"

namespace pio
{	
	void Mesh::setUp(Ref<RenderContext>& context, const TransformComponent &transComp)
	{
		if (!m_buffer.valid())
		{
			MeshBuffer::Create(context, m_buffer, m_triMesh.Vertices, m_triMesh.Indices);
			Ref<UniformData> transform = CreateRef<UniformData, UniformMat4>("transform");
			m_uniforms.insert({ transform->name(), transform });
		}
		m_transform.Euler = transComp.Rotation;
		m_transform.Scale = transComp.Scale;
		m_transform.Position = transComp.Position;

		m_uniforms["transform"]->write(&m_transform.mat());
	}

	template<>
	bool Asset::is<Mesh>() const { return assetType() == AssetType::Mesh; }
}