#include "Node.h"

#include "scene/Scene.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MeshNode"

namespace pio
{
	uint32_t Node::k_NodeNum = 0;	

	Node::Node(Ref<RenderContext>& context, Ref<Scene>& scene, const entt::entity& key, const std::string& name)
		: m_key(key), m_scene(scene), m_uuid(), m_context(context), m_name(name)
	{
		k_NodeNum++;
	}

	Node::Node(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<Node>& parent, const entt::entity& key, const std::string& name)
		: m_key(key), m_scene(scene), m_parent(parent), m_uuid(), m_context(context), m_name(name)
	{
		k_NodeNum++;
	}

	entt::registry* Node::getRegistry() const
	{
		auto scene = m_scene.lock();
		if (scene)
		{
			return &scene->registry().m_registry;
		}
		LOGE("err! scene is invalid");
		std::abort();
		return nullptr;
	}

	void Node::InsertNode(Ref<Scene>& scene, Ref<Node>& node)
	{
		scene->m_registry.m_nodes[node->idx()] = node;
	}
}