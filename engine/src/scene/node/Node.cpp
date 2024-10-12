#include "Node.h"

#include "scene/Scene.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MeshNode"

namespace pio
{
	uint32_t Node::k_NodeNum = 0;	

	Node::Node() : m_key(), m_uuid()
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