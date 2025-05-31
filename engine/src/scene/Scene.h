#pragma once

#include "asset/Asset.h"
#include "node/Node.h"

namespace pio
{
	class CameraNode;
	class DirectionalLitNode;
	class PhysicsWorld;
	class RenderContext;
	class PointLitGroup;

	class Scene : public Asset
	{
		RTTR_ENABLE(Asset)
	public:	
		Scene(const Ref<RenderContext>& context);
		~Scene() = default;

		virtual void OnAttach();
		virtual void OnDetach();
		
		void Insert(const Ref<Node>& node);
		void Remove(const Ref<Node>& node);
		void RemoveAll();
		Ref<Node> FindNode(uint32_t uid) const;

		void Tick(const Ref<RenderContext>& context);

		bool HaveCameras() const { return !m_Cameras.empty(); }
		const std::vector<Ref<CameraNode>>& GetCameras() const { return m_Cameras; }
		const Ref<CameraNode>& GetMainCamera() const { return m_Cameras.back(); }

		std::map<uint32_t, Ref<Node>>& GetNodes() { return m_Nodes; }
		const std::map<uint32_t, Ref<Node>>& GetNodes() const { return m_Nodes; }

		Ref<DirectionalLitNode>& GetMainLit() { return m_MainLit; }
		const Ref<DirectionalLitNode>& GetMainLit() const { return m_MainLit; }

		Ref<PointLitGroup>& GetPointLitGroup() { return m_PointLitGroup; }
		const Ref<PointLitGroup>& GetPointLitGroup() const { return m_PointLitGroup; }

		const Ref<PhysicsWorld>& GetPhysicsWorld() const { return m_PhysicsWorld; }

		AABB& GetBoundingBox() { return m_BoundingBox; }
		const AABB& GetBoundingBox() const { return m_BoundingBox; }

		Ref<RenderContext> GetRenderContext() { return m_Context.lock(); }

	public:
		template <typename T, typename ... Args>
		Ref<T> CreateNode(const std::string& name = std::string(""), const Args&... args)
		{
			if(!std::is_base_of<Node, T>())
				return Ref<T>();

			auto node = CreateRef<T>(args...);
			node->m_Context = m_Context;
			node->m_Name = name.empty() ? PIO_MAKE_NODE_NAME : name;
			node->m_Scene = Self<Scene>();
			node->Init();
			CreatePhysicsActor(node);
			return node;
		}

		template <typename T, typename ... Args>
		Ref<T> CreateChildNode(const Ref<Node>& parent, const std::string& name = std::string(""), const Args&... args)
		{
			if(!std::is_base_of<Node, T>())
				return Ref<T>();

			auto node = CreateRef<T>(args...);
			node->m_Context = m_Context;
			node->m_Name = name.empty() ? PIO_MAKE_NODE_NAME : name;
			if(parent->IsAttached()) { node->m_Scene = parent->m_Scene; }
			node->Init();

			parent->InsertChild(node);
			if(parent->IsAttached()) { node->AttachScene(parent->GetScene()); }			
			CreatePhysicsActor(node);
			return node;
		}

	protected:
		void OnBeforeTick(const Ref<RenderContext>& context);
		void OnTick(const Ref<RenderContext>& context);
		void OnPostTick(const Ref<RenderContext>& context);

	private:
		void CreatePhysicsActor(const Ref<Node>& node);

	private:
		WeakRef<RenderContext> m_Context;
		Ref<PhysicsWorld> m_PhysicsWorld;
		AABB m_BoundingBox;

		std::map<uint32_t, Ref<Node>> m_Nodes;
		std::vector<Ref<CameraNode>> m_Cameras;
		Ref<DirectionalLitNode> m_MainLit;
		Ref<PointLitGroup> m_PointLitGroup;
	};
}