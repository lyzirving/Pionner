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

	void MeshNode::update(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{
		MeshRenderer* render = getComponent<MeshRenderer>();
		TransformComponent* transComp = getComponent<TransformComponent>();

		Ref<Material> material = AssetMgr::GetRuntimeAsset<Material>(render->MatHnd);
		Ref<MeshRenderBuffer> buff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(render->BuffHnd);
		
		buff->Transform.setPosition(transComp->Position);
		buff->Transform.setEuler(transComp->Rotation);
		buff->Transform.setScale(transComp->Scale);

		material->update(context);
		buff->update(context);
	}

	void MeshNode::onInit()
	{
		addComponent<MeshFilter, MeshRenderer, TransformComponent>();
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

	template<>
	MeshRenderingItem Node::getRenderingData<MeshNode>() const
	{
		MeshRenderingItem item;
		if (is<MeshNode>())
		{			
			auto* render = getComponent<MeshRenderer>();
			item.Mode = AssetMgr::GetRuntimeAsset<Material>(render->MatHnd)->renderingMode();
			item.RenderBuffFilter = render->BuffHnd;
			item.MaterialFilter = render->MatHnd;			
		}
		return item;
	}
}