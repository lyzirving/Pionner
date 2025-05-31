#pragma once

#include "component/Component.h"

namespace pio
{
	class Scene;
	class RenderContext;
	class PhysicsActor;
	class NodeGroup;

#define PIO_MAKE_NODE_NAME (std::string("Node").append(std::to_string(Node::k_NodeNum)))

	class Node : public Object
	{
		RTTR_ENABLE(Object)
	public:
		Node();
		virtual ~Node();
					
		virtual void OnSelect() {}
		virtual void OnDeselect() {}		

		/*
		* @brief: insert a child into current node
		* @param node: child to be added
		* @return: the index of child in current node. -1 means fail.
		*/
		int32_t InsertChild(const Ref<Node>& node);
		/*
		* @brief: remove a child from current node
		* @param node: child to be added
		* @return: the index of child in current node. -1 means fail.
		*/
		int32_t RemoveChild(const Ref<Node>& node); 
		Ref<Node> FindChild(uint32_t uid) const;
		void RemoveAllChildren();

		void Init();
		void AttachActor(const Ref<PhysicsActor>& actor);
		void AttachScene(const Ref<Scene>& scene);
		void DetachScene();

		void Tick(const Ref<RenderContext>& context);
		void Render(const Ref<RenderContext>& context);

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);
		void SetScale(const glm::vec3& scale);
		void SetMobility(Mobility mobility);

		const glm::vec3& GetPosition() const;
		const Rotator& GetRotator() const;
		const glm::vec3& GetScale() const;
		Mobility GetMobility() const;
		
		void SetGroupIndex(int32_t index);
		void SetVisible(bool val) { m_Visible = val; }		
		void SetParent(const Ref<Node>& node) { m_Parent = node; }
		void SetShowOutliner(bool show) { m_bShowInOutliner = show; }	
		bool IsChild() const { return !m_Parent.expired(); }
		bool IsAttached() const { return !m_Scene.expired(); }
		bool IsVisible() const { return m_Visible; }
		bool IsInGroup() const;
		bool IsShowOutliner() const { return m_bShowInOutliner; }
		
		Ref<Scene> GetScene() const { return m_Scene.lock(); }
		Ref<Node> GetParent() const { return m_Parent.lock(); }
		template <typename T = NodeGroup>
		T* GetGroup() const { return m_Parent.lock()->As<T>(); }
		const std::string& GetName() const { return m_Name; }
		const std::vector<Ref<Node>>& Children() const { return m_Children; }		

	protected:
		virtual void OnInit() {}

		// ---------- physics related ----------		
		virtual void OnAttachActor() {}
		// -------------------------------------

		// ----------- scene related -----------		
		virtual void OnAttachScene(const Ref<Scene>& scene) {}
		virtual void OnDetachScene() {}
		// -------------------------------------

		// ----------- group related -----------		
		virtual void OnAttachGroup() {}
		virtual void OnDetachGroup() {}
		virtual void OnGroupIndexChange() {}
		// -------------------------------------

		virtual void OnBeginFrameTick(const Ref<RenderContext>& context) {}
		virtual void OnTickComponents(const Ref<RenderContext>& context);
		virtual void OnTick(const Ref<RenderContext>& context) {}
		virtual void OnTickChildren(const Ref<RenderContext>& context);
		virtual void OnEndFrameTick(const Ref<RenderContext>& context) {}
		virtual void OnRender(const Ref<RenderContext>& context) {}
		virtual void OnRenderChildren(const Ref<RenderContext>& context);

	public:
		template <typename T, typename ... Comps>
		Ref<T> AddComponent()
		{
			auto clazz = rttr::type::get<T>();
			std::string name = clazz.get_name().data();
			Ref<Component> comp = CreateRef<T>();
			m_Components[name] = comp;
			comp->m_Host = Self<Node>();
			comp->m_Context = m_Context;
			comp->OnAttach();
			if constexpr(sizeof...(Comps) > 0)
			{
				AddComponent<Comps...>();
			}
			return RefCast<Component, T>(comp);
		}

		template <typename T>
		void RemoveComponent()
		{
			auto clazz = rttr::type::get<T>();
			std::string name = clazz.get_name().data();
			auto it = m_Components.find(name);
			if(it != m_Components.end())
			{
				it->second->OnDetach();
				m_Components.erase(it);
			}
		}

		template <typename T>
		Ref<T> GetComponent() const
		{
			auto clazz = rttr::type::get<T>();
			std::string name = clazz.get_name().data();
			auto it = m_Components.find(name);
			if(it != m_Components.end())
			{
				return RefCast<Component, T>(it->second);
			}
			return Ref<T>();
		}

		template <typename T>
		bool HaveComponent() const
		{
			auto clazz = rttr::type::get<T>();
			std::string name = clazz.get_name().data();
			auto it = m_Components.find(name);
			return it != m_Components.end();
		}

	private:
		static uint32_t k_NodeNum;

		friend class Scene;
		friend class NodeGroup;

	protected:
		WeakRef<Scene> m_Scene;
		WeakRef<RenderContext> m_Context;
		WeakRef<Node> m_Parent;
		WeakRef<PhysicsActor> m_Actor;

		std::string m_Name;
		std::vector<Ref<Node>> m_Children;
		std::unordered_map<std::string, Ref<Component>> m_Components;

		// node's index in a group
		int32_t m_GroupIndex{ -1 };
		bool m_Visible{ true };
		bool m_bShowInOutliner{ true };
	};
}