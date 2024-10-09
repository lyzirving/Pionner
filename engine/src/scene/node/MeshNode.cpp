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
	PIO_NODE_IMPL_CONSTRUCOR(MeshNode, Node)

	MeshNode::~MeshNode() = default;

	void MeshNode::onInit()
	{
		addComponent<MeshFilter, MeshRenderer, TransformComponent>();
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

		if (render->SdCastMode == ShadowCastMode_ShadowOnly)
		{
			return;
		}

		TransformComponent* transComp = getComponent<TransformComponent>();
		Ref<MeshRenderBuffer> renderBuff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(render->BuffHnd);
		renderBuff->Transform.setPosition(transComp->Position);
		renderBuff->Transform.setEuler(transComp->Rotation);
		renderBuff->Transform.setScale(transComp->Scale);
		renderBuff->onUpdate(context);

		Ref<Material> material = AssetMgr::GetRuntimeAsset<Material>(render->MatHnd);
		material->onUpdate(context);

		MeshRenderingItem item;
		item.Mode = material->renderingMode();
		item.RenderBuffFilter = render->BuffHnd;
		item.MaterialFilter = render->MatHnd;

		renderingData.submitMesh(std::move(item));		
	}

	PIO_NODE_IMPL_CONSTRUCOR(PlaneNode, MeshNode)

	PlaneNode::~PlaneNode() = default;

	void PlaneNode::onInit()
	{
		auto context = m_context.lock();

		auto* meshFilter = getComponent<MeshFilter>();
		auto* meshRender = getComponent<MeshRenderer>();

		auto meshData = Factory::MakePlane();
		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->setData(meshData);

		meshFilter->Type = MeshType::Plane;
		meshFilter->MeshHnd = mesh->assetHnd();
		
		meshRender->MatHnd = context->getMaterial(GpuAttr::Mat::STANDARD, true)->assetHnd();

		auto renderBuff = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>();
		renderBuff->setUp(context, meshData->getVertice(), meshData->getIndice());
		meshRender->BuffHnd = renderBuff->assetHnd();
	}

	PIO_NODE_IMPL_CONSTRUCOR(CubeNode, MeshNode)

	CubeNode::~CubeNode() = default;

	void CubeNode::onInit()
	{
		auto context = m_context.lock();

		auto* meshFilter = getComponent<MeshFilter>();
		auto* meshRender = getComponent<MeshRenderer>();
		auto* transform = getComponent<TransformComponent>();

		auto meshData = Factory::MakeCube();
		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->setData(meshData);

		meshFilter->Type = MeshType::Cube;
		meshFilter->MeshHnd = mesh->assetHnd();

		meshRender->MatHnd = context->getMaterial(GpuAttr::Mat::STANDARD, true)->assetHnd();

		auto renderBuff = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>();
		renderBuff->setUp(context, meshData->getVertice(), meshData->getIndice());
		meshRender->BuffHnd = renderBuff->assetHnd();

		transform->Position = glm::vec3(0.f, 1.f, 0.f);
	}

	template<>
	bool Node::is<MeshNode>() const { return nodeType() == NodeType::Mesh; }

	template<>
	bool Node::is<PlaneNode>() const
	{
		if (is<MeshNode>())
		{
			as<MeshNode>()->meshType() == MeshType::Plane;
		}
		return false;
	}

	template<>
	bool Node::is<CubeNode>() const
	{
		if (is<MeshNode>())
		{
			as<MeshNode>()->meshType() == MeshType::Cube;
		}
		return false;
	}
}