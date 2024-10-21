#include "MeshNode.h"

#include "asset/AssetMgr.h"

#include "scene/Components.h"
#include "scene/Factory.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/material/Material.h"
#include "gfx/resource/MeshRenderBuffer.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MeshNode"

namespace pio
{
	MeshNode::MeshNode() : MovableNode()
	{		
	}

	void MeshNode::onInit()
	{
		MovableNode::onInit();
		addComponent<MeshFilter, MeshRenderer>();
	}

	void MeshNode::onUpdate(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingData& renderingData)
	{
		MeshRenderer* render = getComponent<MeshRenderer>();		
		if (render->SdCastMode != ShadowCastMode_Off)
		{
			ShadowCastItem sdItem;
			sdItem.Mode = render->SdCastMode;
			sdItem.RenderBuffFilter = render->BuffHnd;
			renderingData.submitShadowCastMesh(std::move(sdItem));
		}

		bool bSubmitMesh = render->SdCastMode != ShadowCastMode_ShadowOnly;

		if (!bDirty() && !bSubmitMesh)
		{
			return;
		}

		onUpdateDerived(context, camNode);

		TransformComponent* transComp = getComponent<TransformComponent>();
		Ref<MeshRenderBuffer> renderBuff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(render->BuffHnd);
		renderBuff->Transform.setPosition(transComp->Position);
		renderBuff->Transform.setEuler(transComp->Rotation);
		renderBuff->Transform.setScale(transComp->Scale);
		renderBuff->onUpdate(context);

		Ref<Material> material;
		if (render->MatHnd == InvalidId)
		{
			// for default material
			material = context->getMaterial(GpuAttr::Mat::STANDARD, true);
			render->MatHnd = material->assetHnd();
		}
		else
		{
			material = AssetMgr::GetRuntimeAsset<Material>(render->MatHnd);
		}		
		material->onUpdate(context);

		if (bSubmitMesh)
		{
			MeshRenderingItem item;
			item.Mode = material->renderingMode();
			item.RenderBuffFilter = render->BuffHnd;
			item.MaterialFilter = render->MatHnd;
			renderingData.submitMesh(std::move(item));
		}
	}

	void MeshNode::setMaterial(const Ref<Material>& material)
	{
		auto* meshRender = getComponent<MeshRenderer>();
		if (meshRender->MatHnd != material->assetHnd())
		{
			meshRender->MatHnd = material->assetHnd();
		}
	}

	PlaneNode::PlaneNode(float w, float h) : MeshNode(), m_w(w), m_h(h)
	{
	}

	void PlaneNode::onInit()
	{
		MeshNode::onInit();
		auto context = m_context.lock();

		auto* meshFilter = getComponent<MeshFilter>();
		auto* meshRender = getComponent<MeshRenderer>();

		auto meshData = Factory::MakePlane(m_w, m_h);
		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->setData(meshData);

		meshFilter->Type = MeshType::Plane;
		meshFilter->MeshHnd = mesh->assetHnd();		

		auto renderBuff = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>();
		renderBuff->setUp(context, meshData->getVertice(), meshData->getIndice());
		meshRender->BuffHnd = renderBuff->assetHnd();
	}

	void PlaneNode::setWidth(float w)
	{
		if (Math::Equal(m_w, w))
			return;

		m_w = w;
		invalidate();
	}

	void PlaneNode::setHeight(float h)
	{
		if (Math::Equal(m_h, h))
			return;

		m_h = h;
		invalidate();
	}

	void PlaneNode::onUpdateDerived(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{
		if (bDirty())
		{
			invalidate(false);

			auto* filter = getComponent<MeshFilter>();
			auto* meshRender = getComponent<MeshRenderer>();

			auto mesh = AssetMgr::GetRuntimeAsset<Mesh>(filter->MeshHnd);
			auto meshData = Factory::MakePlane(m_w, m_h);
			mesh->setData(meshData);

			auto buff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(meshRender->BuffHnd);
			context->uploadData(meshData->getVertice().data(), sizeof(Vertex3d) * meshData->getVertice().size(), buff->Vbo);
			context->uploadIndice(meshData->getIndice().data(), sizeof(uint16_t) * meshData->getIndice().size(), meshData->getIndice().size(), buff->Ebo);
		}
	}

	CubeNode::CubeNode(float size) : MeshNode(), m_size(size)
	{
	}

	void CubeNode::onInit()
	{
		MeshNode::onInit();
		auto context = m_context.lock();

		auto* meshFilter = getComponent<MeshFilter>();
		auto* meshRender = getComponent<MeshRenderer>();		

		auto meshData = Factory::MakeCube(m_size);
		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->setData(meshData);

		meshFilter->Type = MeshType::Cube;
		meshFilter->MeshHnd = mesh->assetHnd();

		auto renderBuff = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>();
		renderBuff->setUp(context, meshData->getVertice(), meshData->getIndice());
		meshRender->BuffHnd = renderBuff->assetHnd();		
	}

	SphereNode::SphereNode(float radius, int32_t itr) : MeshNode(), m_radius(radius), m_itr(itr)
	{		
	}

	void SphereNode::onInit()
	{
		MeshNode::onInit();
		auto context = m_context.lock();

		auto* meshFilter = getComponent<MeshFilter>();
		auto* meshRender = getComponent<MeshRenderer>();

		auto meshData = Factory::MakeSphere(m_radius, m_itr);
		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->setData(meshData);

		meshFilter->Type = MeshType::Sphere;
		meshFilter->MeshHnd = mesh->assetHnd();		

		auto renderBuff = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>(); 
		renderBuff->setUp(context, meshData->getVertice(), meshData->getIndice()); 
		meshRender->BuffHnd = renderBuff->assetHnd();
	}

	template<>
	bool Node::is<MeshNode>() const { return nodeType() == NodeType::Mesh; }

	template<>
	bool Node::is<PlaneNode>() const
	{
		if (is<MeshNode>())
		{
			return as<MeshNode>()->meshType() == MeshType::Plane;
		}
		return false;
	}

	template<>
	bool Node::is<CubeNode>() const
	{
		if (is<MeshNode>())
		{
			return as<MeshNode>()->meshType() == MeshType::Cube;
		}
		return false;
	}

	template<>
	bool Node::is<SphereNode>() const
	{
		if(is<MeshNode>())
		{
			return as<MeshNode>()->meshType() == MeshType::Sphere;
		}
		return false;
	}	
}