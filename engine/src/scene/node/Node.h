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
		Gizmo,
		Num
	};

	#define PIO_NODE_MAKE_NAME (std::string("Node") + std::to_string(Node::k_NodeNum))

	#define PIO_NODE_IMPL_TYPE(TypeName)  private:\
									      static  NodeType StaticNodeType() { return TypeName; }\
										  public:\
									      virtual NodeType nodeType() const override { return StaticNodeType(); }

	#define PIO_NODE_DECLARE(Clazz, TypeName) PIO_NODE_IMPL_TYPE(TypeName)											 

	class Scene;
	class CameraNode;
	class RenderContext;
	struct RenderingData;

	class Node : public std::enable_shared_from_this<Node>
	{
		PIO_DECLARE_IS_AS(Node)
	public:
		Node();

		virtual ~Node() = default;
		virtual NodeType nodeType() const = 0;		
		virtual void onInit() {};
		virtual void onAttach(Ref<Scene>& scene) {}
		virtual void onDetach(Ref<Scene>& scene) {}
		virtual void onUpdate(Ref<RenderContext>& context, RenderingData& renderingData) {};
		virtual void onUpdate(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingData& renderingData) {};

		bool operator==(const Node& rhs) { return this->m_key == rhs.m_key && this->m_uuid == rhs.m_uuid; }

		void setVisible(bool visible) { m_visible = visible; }

		template<typename T>
		Ref<T> self() { return RefCast<Node, T>(shared_from_this()); }

		//Note function should not be called in Node's constructor
		template<typename T, typename ... Args>
		Ref<Node> addChild(const std::string& name, const Args&... args) 
		{
			auto ctx = m_context.lock();
			auto scene = m_scene.lock();
			if (!ctx || !scene)
				return Ref<Node>();
			
			auto parent = self<Node>();
			Ref<Node> node = CreateRef<T>(args...);
			node->m_context = ctx;
			node->m_scene = scene;
			node->m_parent = parent;
			node->m_key = getRegistry()->create();
			node->m_name = name.empty() ? PIO_NODE_MAKE_NAME : name;
			node->onInit();

			m_children.push_back(node);
			InsertNode(scene, node);
			return node;
		}

		template <typename T, typename ... Comps>
		void addComponent()
		{
			auto* regi = getRegistry();
			regi->emplace<T>(m_key);
			if constexpr (sizeof...(Comps) > 0)
			{
				addComponent<Comps...>();
			}
		}

		template <typename T, typename ... Comps>
		uint32_t removeComponent()
		{
			return getRegistry()->remove(T, Comps...)(m_key);
		}

		template <typename T>
		T* getComponent() const
		{
			return getRegistry()->try_get<T>(m_key);
		}

		template <typename T>
		bool has() const 
		{ 
			return getRegistry()->any_of<T>(m_key);
		}

		Ref<Node> getParent() const { return m_parent.lock(); }
		bool isChild() const { return getParent().use_count() != 0; }
		entt::entity key() const { return m_key; }
		uint32_t idx() const { return (uint32_t)m_key; }		
		uint32_t uuid() const { return m_uuid; }
		const std::string& name() const { return m_name; }
		const std::vector<Ref<Node>>& children() const { return m_children; }
		bool bShowInInspector() const { return m_showInInspector; }
		bool bVisible() const { return m_visible; }

	private:
		friend class Scene;
		friend class Registry;

		entt::registry* getRegistry() const;
		static void InsertNode(Ref<Scene>& scene, Ref<Node>& node);

	public:
		static uint32_t k_NodeNum;

	protected:
		entt::entity m_key;
		UUID32 m_uuid;
		std::string m_name;

		WeakRef<Node> m_parent;
		std::vector<Ref<Node>> m_children;

		WeakRef<Scene> m_scene;
		WeakRef<RenderContext> m_context;

		bool m_visible{ true };
		bool m_showInInspector{ true };
	};
}
#endif 