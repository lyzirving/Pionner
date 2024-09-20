#include "MeshNode.h"

#include "asset/AssetMgr.h"

#include "scene/Components.h"
#include "scene/Factory.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/Material.h"
#include "gfx/resource/MeshRenderBuffer.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MeshNode"

namespace pio
{
	MeshNode::MeshNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name)
		: Node(context, key, regi, name)
	{
		addComponent<MeshFilter, MeshRenderer, TransformComponent>();
	}

	MeshNode::~MeshNode() = default;

	void MeshNode::update(Ref<RenderContext>& context)
	{
		MeshFilter* filter = getComponent<MeshFilter>(); 
		MeshRenderer* render = getComponent<MeshRenderer>();
		TransformComponent* transComp = getComponent<TransformComponent>();

		Ref<Mesh> mesh = AssetMgr::GetRuntimeAsset<Mesh>(filter->MeshHnd);
		Ref<Material> material = AssetMgr::GetRuntimeAsset<Material>(render->MatHnd);
		Ref<MeshRenderBuffer> buff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(render->BuffHnd);

		mesh->update(context, *transComp);
		material->update(context);
		buff->update(context, mesh);
	}

	PlaneNode::PlaneNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name)
		: MeshNode(context, key, regi, name)
	{
		auto* meshFilter = getComponent<MeshFilter>();
		auto* meshRender = getComponent<MeshRenderer>();

		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->m_triangles = Factory::MakePlane();

		meshFilter->Type = MeshType::Plane;
		meshFilter->MeshHnd = mesh->assetHnd();

		meshRender->MatHnd = context->getMaterial(GpuAttr::Mat::STANDARD, true)->assetHnd();
		meshRender->BuffHnd = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>()->assetHnd();
	}

	PlaneNode::~PlaneNode() = default;

	CubeNode::CubeNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name)
		: MeshNode(context, key, regi, name)
	{
		auto* meshFilter = getComponent<MeshFilter>();
		auto* meshRender = getComponent<MeshRenderer>();
		auto* transform = getComponent<TransformComponent>();

		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->m_triangles = Factory::MakeCube();

		meshFilter->Type = MeshType::Cube;
		meshFilter->MeshHnd = mesh->assetHnd();

		meshRender->MatHnd = context->getMaterial(GpuAttr::Mat::STANDARD, true)->assetHnd();
		meshRender->BuffHnd = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>()->assetHnd();

		transform->Position = glm::vec3(0.f, 1.f, 0.f);
	}

	CubeNode::~CubeNode() = default;

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
		if (is<MeshNode>())
		{
			MeshRenderingItem item;
			auto* render = getComponent<MeshRenderer>();
			item.Mode = AssetMgr::GetRuntimeAsset<Material>(render->MatHnd)->renderingMode();
			item.RenderBuffFilter = render->BuffHnd;
			item.MaterialFilter = render->MatHnd;
			return item;
		}
		return MeshRenderingItem();
	}
}