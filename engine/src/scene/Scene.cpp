#include "Scene.h"

#include "scene/node/CameraNode.h"
#include "scene/node/DirectionalLitNode.h"
#include "scene/node/PointLitNode.h"
#include "scene/node/PointLitGroup.h"

#include "physics/PhysicsWorld.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Scene"

namespace pio
{
	Scene::Scene(const Ref<RenderContext>& context) : Asset(), m_Context(context)
	{
		m_PhysicsWorld = CreateRef<PhysicsWorld>(context);
	}

	void Scene::OnAttach()
	{
		m_PointLitGroup = CreateNode<PointLitGroup>("PointLitGroup");
	}

	void Scene::OnDetach()
	{
		RemoveAll();
	}

	void Scene::Insert(const Ref<Node>& node)
	{
		if(!node)
			return;

		auto it = m_Nodes.find(node->UId());
		if(it != m_Nodes.end())
			return;

		m_Nodes.insert({ node->UId(), node });
		node->AttachScene(Self<Scene>());

		if(node->Is<CameraNode>())
		{
			m_Cameras.push_back(RefCast<Node, CameraNode>(node));
		}
		else if(node->Is<DirectionalLitNode>())
		{
			m_MainLit = RefCast<Node, DirectionalLitNode>(node);
		}
		else if(node->Is<PointLitNode>())
		{
			while(m_PointLitGroup->Size() >= PIO_POINT_LIT_LIMIT)
			{
				Remove(m_PointLitGroup->PopFront());
			}
			m_PointLitGroup->GroupInsert(node);
		}
	}

	void Scene::Remove(const Ref<Node>& node)
	{
		if(!node)
			return;

		auto it = m_Nodes.find(node->UId());
		if(it == m_Nodes.end())
			return;

		it->second->DetachScene();
		m_Nodes.erase(it);

		if(node->Is<CameraNode>())
		{
			auto it = std::find(m_Cameras.begin(), m_Cameras.end(), node);
			m_Cameras.erase(it);
		}
		else if(node->Is<DirectionalLitNode>())
		{
			if(m_MainLit == node)
				m_MainLit.reset();
		}
		else if(node->Is<PointLitNode>())
		{
			m_PointLitGroup->GroupRemoval(node);
		}
	}

	void Scene::RemoveAll()
	{
		auto it = m_Nodes.begin();
		while(it != m_Nodes.end())
		{
			it->second->DetachScene();
			it = m_Nodes.erase(it);
		}

		m_Cameras.clear();
		m_MainLit.reset();
		m_PointLitGroup->RemoveAllChildren();
	}

	Ref<Node> Scene::FindNode(uint32_t uid) const
	{
		auto it = m_Nodes.find(uid);
		if(it != m_Nodes.end())
			return it->second;

		return Ref<Node>();
	}

	void Scene::Tick(const Ref<RenderContext>& context)
	{
		OnBeforeTick(context);
		OnTick(context);
		OnPostTick(context);
	}

	void Scene::OnBeforeTick(const Ref<RenderContext>& context)
	{
		m_BoundingBox.Reset();
	}

	void Scene::OnTick(const Ref<RenderContext>& context)
	{
		for(auto& it : m_Nodes)
		{
			if(it.second->Is<DirectionalLitNode>())
				continue;

			it.second->Tick(context);
			it.second->Render(context);
		}

		if(m_MainLit)
		{
			m_MainLit->Tick(context);
			m_MainLit->Render(context);
		}

		if(m_PointLitGroup)
		{
			m_PointLitGroup->Tick(context);
			m_PointLitGroup->Render(context);
		}
	}

	void Scene::OnPostTick(const Ref<RenderContext>& context)
	{
		auto& data = context->GetRenderingData();
		if(!data.TransparentMeshItems.empty() && !m_Cameras.empty())
		{
			//sort transparent items, and make it listed from far to near
			std::sort(data.TransparentMeshItems.begin(), data.TransparentMeshItems.end(), 
					  [camera = m_Cameras[0]](const MeshRenderingItem& lhs, MeshRenderingItem& rhs)
			{
				float lhsDist = glm::distance(lhs.Center, camera->GetPosition());
				float rhsDist = glm::distance(rhs.Center, camera->GetPosition());
				return lhsDist > rhsDist;
			});
		}
	}

	void Scene::CreatePhysicsActor(const Ref<Node>& node)
	{
		m_PhysicsWorld->CreateActor(node);
	}
}