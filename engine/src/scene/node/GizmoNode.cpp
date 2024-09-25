#include "GizmoNode.h"

#include "scene/Factory.h"
#include "scene/Components.h"

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
		addComponent<MeshFilter, MeshRenderer>();
		auto* meshFilter = getComponent<MeshFilter>();
		auto* meshRenderer = getComponent<MeshRenderer>();

		meshFilter->Type = MeshType::Outline;
	}

	void DirLightGizmo::onAttach(Ref<Scene>& scene)
	{
	}
}