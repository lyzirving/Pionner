#ifndef __PIONNER_SCENE_NODE_LIGHT_NODE_H__
#define __PIONNER_SCENE_NODE_LIGHT_NODE_H__

#include "Node.h"
#include "gfx/GfxDef.h"

namespace pio
{
	#define PIO_LIGHT_DEFINE_TYPE(TypeName)  private:\
									         static LightType StaticLightType() { return TypeName; }\
											 public:\
									         virtual LightType lightType() const override { return StaticLightType(); }
	
	class Camera;
	class CameraNode;
	class UniformBuffer;
	struct DirectionalLightUD;
	struct DirectionalLightShadowDataUD;

	class LightNode : public Node
	{
		PIO_NODE_DECLARE(LightNode, NodeType::Light)
	public:
		virtual ~LightNode();
		virtual LightType lightType() const = 0;
	};

	class DirectionalLightNode : public LightNode
	{
		PIO_NODE_DECLARE_CONSTRUCTOR(DirectionalLightNode)
		PIO_LIGHT_DEFINE_TYPE(LightType::DirectionLight)
	public:
		~DirectionalLightNode();		
		virtual void onInit() override;
		virtual void onAttach(Ref<Scene>& scene) override;
		virtual void onUpdate(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingData& renderingData) override;

		Ref<UniformBuffer> getShadowData() const { return m_UBufferShadow; }

	private:
		void updateLight(Ref<RenderContext>& context, Ref<CameraNode>& camNode);
		void updateShadow(Ref<RenderContext>& context, Ref<CameraNode>& camNode);

	private:
		friend class Node;

	private:
		Ref<DirectionalLightUD> m_UData;
		Ref<DirectionalLightShadowDataUD> m_UDataShadow;
		Ref<UniformBuffer> m_UBuffer, m_UBufferShadow;
		Ref<Camera> m_shadowCam;
	};

	template<>
	bool Node::is<LightNode>() const;

	template<>
	bool Node::is<DirectionalLightNode>() const;
}

#endif