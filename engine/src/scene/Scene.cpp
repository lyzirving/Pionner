#include "Scene.h"
#include "Entity.h"
#include "Components.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/pipeline/RenderPipeline.h"
#include "gfx/pipeline/PipelineUtils.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Scene"

namespace pio
{
	void Scene::onAttach()
	{
	}

	void Scene::onDetach()
	{
		removeAllEntities();
	}

	void Scene::onUpdate(Ref<RenderContext>& context, Ref<RenderPipeline>& pipeline)
	{
		RenderingEntities data;
		data.Mesh = m_registry.view<MeshFilter, MeshRenderer>();

		auto cameraEntities = m_registry.view<CameraComponent>();
		auto cameras = pipeline::FetchCamera(context, cameraEntities);

		context->setRenderingEntities(std::move(data));
		pipeline->onRender(context, cameras);
	}

	void Scene::removeEntity(Ref<Entity>& ent)
	{
		if (!ent)
			return;

		for (size_t i = 0; i < m_ents.size(); i++)
		{
			if (m_ents[i] && ent && *m_ents[i].get() == *ent.get())
			{
				m_ents.erase(m_ents.begin() + i);
				m_registry.destroy(ent);
				return;
			}
		}
	}

	void Scene::removeAllEntities()
	{
		auto it = m_ents.begin();
		while (it != m_ents.end())
		{
			m_registry.destroy((*it));
			it = m_ents.erase(it);
		}
	}

	template<>
	bool Asset::is<Scene>() const { return assetType() == AssetType::SceneResource; }
}