#include "Scene.h"
#include "Components.h"

#include "scene/node/MeshNode.h"
#include "scene/node/LightNode.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/pipeline/RenderPipeline.h"

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
		removeAllNodes();
	}

	void Scene::onUpdate(Ref<RenderContext>& context, Ref<RenderPipeline>& pipeline, std::vector<Ref<CameraNode>>& camNodes)
	{
		RenderingNodes data;
		auto lights = m_registry.view<DirectionalLightNode, DirectionalLightComponent>();

		data.Mesh = m_registry.view<MeshNode, MeshFilter, MeshRenderer>();
		data.MainLight = lights.empty() ? Ref<DirectionalLightNode>() : lights[0];

		context->setRenderingNodes(std::move(data));
		pipeline->onRender(context, camNodes);
	}

	void Scene::removeNode(Ref<Node>& node)
	{
		if (!node)
			return;

		for (size_t i = 0; i < m_nodes.size(); i++)
		{
			if (m_nodes[i] && node && *m_nodes[i].get() == *node.get())
			{
				m_nodes.erase(m_nodes.begin() + i);
				m_registry.destroy(node);
				return;
			}
		}
	}

	void Scene::removeAllNodes()
	{
		auto it = m_nodes.begin();
		while (it != m_nodes.end())
		{
			m_registry.destroy((*it));
			it = m_nodes.erase(it);
		}
	}

	template<>
	bool Asset::is<Scene>() const { return assetType() == AssetType::SceneResource; }
}