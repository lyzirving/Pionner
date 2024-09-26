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
	PIO_NODE_IMPL_CONSTRUCOR(GizmoNode, Node)

	GizmoNode::~GizmoNode() = default;

	PIO_NODE_IMPL_CONSTRUCOR(DirLightGizmo, GizmoNode)

	DirLightGizmo::~DirLightGizmo() = default;

	void DirLightGizmo::update(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{
	}

	void DirLightGizmo::onInit()
	{
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

		Ref<LineSeqMesh> meshData = Factory::MakeDirLightGizmo(0.5f, 2.f);
		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->setData(meshData);

		meshFilter->Type = MeshType::Outline;
		meshFilter->MeshHnd = mesh->assetHnd();

		auto renderBuff = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>();
		renderBuff->setUp(context, meshData->getVertice(), meshData->getIndice());
		meshRender->BuffHnd = renderBuff->assetHnd();

		auto colorMat = context->getMaterial(GpuAttr::Mat::COLOR, true);
		colorMat->as<ColorMaterial>()->setColor(glm::vec3(0.964f, 0.953f, 0.051f));
		meshRender->MatHnd = colorMat->assetHnd();
	}
}