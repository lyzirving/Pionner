#ifndef __PIONNER_SCENE_NODE_GIZMO_NODE_H__
#define __PIONNER_SCENE_NODE_GIZMO_NODE_H__

#include "Node.h"
#include "gfx/resource/RenderingData.h"

namespace pio
{
	#define PIO_GIZIMO_DEFINE_TYPE(TypeName)  private:\
											  static GizmoType StaticGizmoType() { return TypeName; }\
											  public:\
									          virtual GizmoType gizmoType() const override { return StaticGizmoType(); }

	class GizmoNode : public Node
	{
		PIO_NODE_DECLARE(GizmoNode, NodeType::Gizmo)
	public:
		virtual ~GizmoNode();
		virtual GizmoType gizmoType() const = 0;
		virtual void onInit() override;		
	};

	class DirLightGizmo : public GizmoNode
	{
		PIO_NODE_DECLARE_CONSTRUCTOR(DirLightGizmo)
		PIO_GIZIMO_DEFINE_TYPE(GizmoType::DirLight)
	public:
		~DirLightGizmo();
	
		virtual void onInit() override;
		virtual void onUpdate(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingData& renderingData) override;

	private:
		MeshRenderingItem onUpdateInner(Ref<RenderContext>& context, Ref<CameraNode>& camNode);
	};

	template<>
	bool Node::is<GizmoNode>() const;

	template<>
	bool Node::is<DirLightGizmo>() const;
}

#endif