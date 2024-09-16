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
		Ref<Node> create(Ref<RenderContext>& context, const std::string& name = "")
		{
			Ref<Node> node = CreateRef<T>(context, m_registry.create(), m_registry, name.empty() ? PIO_NODE_MAKE_NAME : name);
			m_nodes[node->idx()] = node;
			return node;
		}

		template<typename T, typename... Comps>
		std::vector<Ref<T>> view() const
		{
			std::vector<Ref<T>> nodes;
			auto v = m_registry.view<Comps...>();
			for (auto n : v)
			{
				auto it = m_nodes.find((uint32_t)n);
				if (it != m_nodes.end())
				{
					auto real = it->second->self<T>();
					if (real)
					{
						nodes.push_back(real);
					}
				}
			}
			return nodes;
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

		const Ref<Node>& getNode(uint32_t index) const
		{
			auto it = m_nodes.find(index);
			if (it != m_nodes.end())
			{
				return it->second;
			}
			return Ref<Node>();
		}

	private:
		entt::registry m_registry;
		std::unordered_map<uint32_t, Ref<Node>> m_nodes;
	};
}

#endif