#include "GizmoNode.h"

#include "asset/AssetMgr.h"

#include "scene/Factory.h"
#include "scene/Components.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/MeshRenderBuffer.h"
#include "gfx/resource/material/ColorMaterial.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GizmoNode"

namespace pio
{
	GizmoNode::GizmoNode() : Node()
	{		
	}

	void GizmoNode::onInit()
	{
		Node::onInit();
		m_showInInspector = false;
	}


	DirLightGizmo::DirLightGizmo() : GizmoNode()
	{		
	}

	void DirLightGizmo::onInit()
	{	
		GizmoNode::onInit();

		auto parent = m_parent.lock();
		if (!parent)
		{
			LOGE("err! DirLightGizmo must be used as a child");
			std::abort();
		}
		auto context = m_context.lock();

		addComponent<MeshFilter, MeshRenderer>();
		auto* meshFilter = getComponent<MeshFilter>();
		auto* meshRender = getComponent<MeshRenderer>();

		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		auto renderBuff = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>();
		Ref<LineSeqMesh> meshData = Factory::MakeDirLightGizmo(0.5f, 2.f);		
		mesh->setData(meshData);
		renderBuff->setUp(context, meshData->getVertice(), meshData->getIndice());

		meshRender->BuffHnd = renderBuff->assetHnd();
		meshFilter->Type = MeshType::Outline;
		meshFilter->MeshHnd = mesh->assetHnd();

		auto colorMat = context->getMaterial(GpuAttr::Mat::COLOR, true);
		colorMat->as<ColorMaterial>()->setColor(glm::vec3(0.964f, 0.953f, 0.051f));
		meshRender->MatHnd = colorMat->assetHnd();
	}

	void DirLightGizmo::onUpdate(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingData& renderingData)
	{
		auto item = onUpdateInner(context, camNode);
		renderingData.submitOutline(std::move(item));
	}

	MeshRenderingItem DirLightGizmo::onUpdateInner(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{
		MeshRenderingItem item;
		auto parent = m_parent.lock();
		if (!parent)
		{
			LOGW("warning! gizmo's parent is invalid");
			return item;
		}
		auto* parentTransComp = parent->getComponent<TransformComponent>();
		if (!parentTransComp)
		{
			LOGW("warning! gizmo's parent doesn't have TransformComponent");
			return item;
		}
		MeshRenderer* render = getComponent<MeshRenderer>();
		Ref<MeshRenderBuffer> renderBuff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(render->BuffHnd);

		Transform3D transform;
		transform.setEuler(parentTransComp->Rotation);
		auto quat = glm::quat_cast(glm::mat3(glm::rotate(transform.rotMat(), glm::radians(180.f), World::Up)));

		renderBuff->Transform.setPosition(parentTransComp->Position);
		renderBuff->Transform.setEuler(glm::degrees(glm::eulerAngles(quat)));
		renderBuff->Transform.setScale(parentTransComp->Scale);
		renderBuff->onUpdate(context);

		Ref<Material> material = AssetMgr::GetRuntimeAsset<Material>(render->MatHnd);
		material->onUpdate(context);

		item.Mode = material->renderingMode();
		item.RenderBuffFilter = render->BuffHnd;
		item.MaterialFilter = render->MatHnd;

		return item;
	}

	template<>
	bool Node::is<GizmoNode>() const { return nodeType() == NodeType::Gizmo; }

	template<>
	bool Node::is<DirLightGizmo>() const 
	{
		if (is<GizmoNode>())
		{
			return as<GizmoNode>()->gizmoType() == GizmoType::DirLight;
		}
		return false;
	}
}