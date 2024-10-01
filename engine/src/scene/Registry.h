#ifndef __PIONNER_SCENE_REGISTRY_H__
#define __PIONNER_SCENE_REGISTRY_H__

#include "scene/node/Node.h"

namespace pio
{
	class Registry
	{
	public:
		Registry() {}
		~Registry() { shutdown(); }

		template <typename T>
		Ref<Node> create(Ref<RenderContext>& context, Ref<Scene> scene, const std::string& name = "")
		{
			Ref<Node> node = CreateRef<T>(context, scene, m_registry.create(), name.empty() ? PIO_NODE_MAKE_NAME : name);
			m_nodes[node->idx()] = node;
			return node;
		}

		/*
		* @brief: Get target nodes of type T width target components
		*/
		template<typename T, typename... Comps>
		std::vector<Ref<T>> view() const
		{
			std::vector<Ref<T>> nodes;
			auto v = m_registry.view<Comps...>();
			for (auto n : v)
			{
				auto it = m_nodes.find((uint32_t)n);
				if (it != m_nodes.end() && it->second->is<T>() && it->second->bVisible())
				{
					auto real = it->second->self<T>();
					nodes.push_back(real);
				}
			}
			return nodes;
		}

		template<typename T = Node>
		Ref<T> getNode(uint32_t index) const
		{
			auto it = m_nodes.find(index);
			if (it != m_nodes.end())
			{
				if (typeid(T) == typeid(Node))
				{
					return it->second;
				}
				return it->second->self<T>();
			}
			return Ref<T>();
		}

		void destroy(Ref<Node>& node)
		{
			m_nodes.erase(node->idx());
			m_registry.destroy(node->key());
		}

		void shutdown()
		{
			auto it = m_nodes.begin();
			while (it != m_nodes.end())
			{
				m_registry.destroy(it->second->key());
				it = m_nodes.erase(it);
			}
		}

	private:
		friend class Node;

	private:
		entt::registry m_registry;
		std::unordered_map<uint32_t, Ref<Node>> m_nodes;
	};
}

#endif