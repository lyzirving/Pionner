#ifndef __PIONNER_SCENE_NODE_NODE_H__
#define __PIONNER_SCENE_NODE_NODE_H__

#include <entt/entity/registry.hpp>
#include "Common.h"

namespace pio
{
	enum class NodeType : uint8_t
	{
		Camera,
		Mesh,
		Light,
		Sprite,
		Num
	};

	#define PIO_NODE_MAKE_NAME (std::string("Node") + std::to_string(Node::k_NodeNum))

	#define PIO_DEFINE_NODE_TYPE(TypeName)  public:\
									        static  NodeType StaticNodeType() { return TypeName; }\
									        virtual NodeType nodeType() const override { return StaticNodeType(); }
	class RenderContext;

	class Node : public std::enable_shared_from_this<Node>
	{
		PIO_DECLARE_IS_AS(Node)
	public:
		Node(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name);

		virtual ~Node() = default;
		virtual NodeType nodeType() const = 0;		
		virtual void update(Ref<RenderContext>& context) = 0;

		bool operator==(const Node& rhs) { return this->m_key == rhs.m_key && this->m_uuid == rhs.m_uuid; }

		template<typename T>
		Ref<T> self() { return RefCast<Node, T>(shared_from_this()); }

		template<typename T, typename U>
		U getRenderingData() const { return U(); }

		template <typename T, typename ... Comps>
		void addComponent()
		{
			m_regi.emplace<T>(m_key);
			if constexpr (sizeof...(Comps) > 0)
			{
				addComponent<Comps...>();
			}
		}

		template <typename T, typename ... Comps>
		uint32_t removeComponent()
		{
			return m_regi.remove(T, Comps...)(m_key);
		}

		template <typename T>
		T* getComponent() const
		{
			return m_regi.try_get<T>(m_key);
		}

		template <typename T>
		bool has() const 
		{ 
			return m_regi.any_of<T>(m_key);
		}

		entt::entity key() const { return m_key; }
		uint32_t idx() const { return (uint32_t)m_key; }		
		uint32_t uuid() const { return m_uuid; }
		const std::string& name() const { return m_name; }
		const std::vector<Ref<Node>>& children() const { return m_children; }

	public:
		static uint32_t k_NodeNum;

	protected:
		const entt::entity m_key;
		entt::registry& m_regi;
		UUID32 m_uuid;
		std::string m_name;
		std::vector<Ref<Node>> m_children;
		WeakRef<RenderContext> m_context;
	};
}
#endif 