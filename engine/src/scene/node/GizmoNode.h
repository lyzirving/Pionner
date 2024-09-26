#ifndef __PIONNER_SCENE_NODE_GIZMO_NODE_H__
#define __PIONNER_SCENE_NODE_GIZMO_NODE_H__

#include "Node.h"

namespace pio
{
	class GizmoNode : public Node
	{
		PIO_NODE_DECLARE(GizmoNode, NodeType::Gizmo)
	public:
		virtual ~GizmoNode();
	};

	class DirLightGizmo : public GizmoNode
	{
		PIO_NODE_DECLARE_CONSTRUCTOR(DirLightGizmo)
	public:
		~DirLightGizmo();

		virtual void update(Ref<RenderContext>& context, Ref<CameraNode>& camNode) override;
		virtual void onInit() override;
	};
}

#endif