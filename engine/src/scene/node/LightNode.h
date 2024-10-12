#ifndef __PIONNER_SCENE_NODE_LIGHT_NODE_H__
#define __PIONNER_SCENE_NODE_LIGHT_NODE_H__

#include "MovableNode.h"
#include "gfx/GfxDef.h"

namespace pio
{
	#define PIO_LIGHT_DEFINE_TYPE(TypeName)  private:\
									         static LightType StaticLightType() { return TypeName; }\
											 public:\
									         virtual LightType lightType() const override { return StaticLightType(); }
	
	class CameraNode;
	class UniformBuffer;
	class GizmoNode;
	struct DirectionalLightUD;

	class LightNode : public MovableNode
	{
		PIO_NODE_DECLARE(LightNode, NodeType::Light)
	public:
		LightNode();
		virtual ~LightNode() = default;
		virtual void onInit() override;
		virtual LightType lightType() const = 0;
	};

	class DirectionalLightNode : public LightNode
	{		
		PIO_LIGHT_DEFINE_TYPE(LightType::DirectionLight)
	public:
		DirectionalLightNode();
		~DirectionalLightNode() = default;
		virtual void onInit() override;
		virtual void onAttach(Ref<Scene>& scene) override;
		virtual void onUpdate(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingData& renderingData) override;

		const glm::vec3& direction() const;		

	private:
		void updateLight(Ref<RenderContext>& context, Ref<CameraNode>& camNode);
		void updateShadow(Ref<RenderContext>& context, Ref<CameraNode>& camNode);

	private:
		friend class Node;

	private:
		Ref<DirectionalLightUD> m_UData;
		Ref<UniformBuffer> m_UBuffer;

		Ref<GizmoNode> m_gizmo;
	};

	template<>
	bool Node::is<LightNode>() const;

	template<>
	bool Node::is<DirectionalLightNode>() const;
}

#endif