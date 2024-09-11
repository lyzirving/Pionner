#include "Mesh.h"

#include "scene/Components.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Mesh"

namespace pio
{
	void Mesh::update(Ref<RenderContext>& context, const TransformComponent& comp)
	{
		m_transform.setEuler(comp.Rotation);
		m_transform.setScale(comp.Scale);
		m_transform.setPosition(comp.Position);
	}

	template<>
	bool Asset::is<Mesh>() const { return assetType() == AssetType::Mesh; }
}