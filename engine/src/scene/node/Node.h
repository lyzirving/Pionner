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

	#define PIO_NODE_IMPL_TYPE(TypeName)  public:\
									      static  NodeType StaticNodeType() { return TypeName; }\
									      virtual NodeType nodeType() const override { return StaticNodeType(); }

	#define PIO_NODE_DECLARE_CONSTRUCTOR(Clazz) public:\
                                                Clazz(Ref<RenderContext>& context, Ref<Scene>& scene, const entt::entity& key, const std::string& name);\
												Clazz(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<Node>& parent, const entt::entity& key, const std::string& name);

	#define PIO_NODE_DECLARE(Clazz, TypeName) PIO_NODE_DECLARE_CONSTRUCTOR(Clazz)\
                                              PIO_NODE_IMPL_TYPE(TypeName)
											  

	#define PIO_NODE_IMPL_CONSTRUCOR(Clazz, ClazzParent) Clazz::Clazz(Ref<RenderContext>& context, Ref<Scene>& scene, const entt::entity& key, const std::string& name)\
												: ClazzParent(context, scene, key, name)\
											{\
												onInit();\
											}\
		                                     \
											Clazz::Clazz(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<Node>& parent, const entt::entity& key, const std::string& name)\
												: ClazzParent(context, scene, parent, key, name)\
											{\
												onInit();\
											}

	class Scene;
	class CameraNode;
	class RenderContext;

	class Node : public std::enable_shared_from_this<Node>
	{
		PIO_DECLARE_IS_AS(Node)
	public:
		Node(Ref<RenderContext>& context, Ref<Scene>& scene, const entt::entity& key, const std::string& name);
		Node(Ref<RenderContext>& context, Ref<Scene>& scene, Ref<Node>& parent, const entt::entity& key, const std::string& name);

		virtual ~Node() = default;
		virtual NodeType nodeType() const = 0;
		virtual void update(Ref<RenderContext>& context) {};
		virtual void update(Ref<RenderContext>& context, Ref<CameraNode>& camNode) {};
		virtual void onInit() {};
		virtual void onAttach(Ref<Scene>& scene) {}
		virtual void onDetach(Ref<Scene>& scene) {}

		bool operator==(const Node& rhs) { return this->m_key == rhs.m_key && this->m_uuid == rhs.m_uuid; }

		template<typename T>
		Ref<T> self() { return RefCast<Node, T>(shared_from_this()); }

		template<typename T, typename U>
		U getRenderingData() const { return U(); }

		//Note function should not be called in Node's constructor
		template<typename T>
		Ref<Node> addChild(const std::string& name = "") 
		{
			auto ctx = m_context.lock();
			auto scene = m_scene.lock();
			if (!ctx || !scene)
				return Ref<Node>();
			
			auto parent = self<Node>();
			Ref<Node> node = CreateRef<T>(ctx, scene, parent, getRegistry()->create(), name.empty() ? PIO_NODE_MAKE_NAME : name);
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

	private:
		friend class Scene;

		entt::registry* getRegistry() const;
		static void InsertNode(Ref<Scene>& scene, Ref<Node>& node);

	public:
		static uint32_t k_NodeNum;

	protected:
		const entt::entity m_key;
		UUID32 m_uuid;
		std::string m_name;

		WeakRef<Node> m_parent;
		std::vector<Ref<Node>> m_children;

		WeakRef<Scene> m_scene;
		WeakRef<RenderContext> m_context;		
	};
}
#endif 