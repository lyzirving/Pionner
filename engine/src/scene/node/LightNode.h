#ifndef __PIONNER_SCENE_NODE_LIGHT_NODE_H__
#define __PIONNER_SCENE_NODE_LIGHT_NODE_H__

#include "Node.h"
#include "gfx/GfxDef.h"

namespace pio
{
	#define PIO_DEFINE_LIGHT_TYPE(TypeName)  public:\
									         static LightType StaticLightType() { return TypeName; }\
									         virtual LightType lightType() const override { return StaticLightType(); }
	
	class UniformBuffer;
	struct DirectionalLightUD;

	class LightNode : public Node
	{
		PIO_DEFINE_NODE_TYPE(NodeType::Light)
	public:
		LightNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name);
		virtual ~LightNode();

		virtual LightType lightType() const = 0;
	};

	class DirectionalLightNode : public LightNode
	{
		PIO_DEFINE_LIGHT_TYPE(LightType::DirectionLight)
	public:
		DirectionalLightNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name);
		~DirectionalLightNode();

		virtual void update(Ref<RenderContext>& context) override;

	private:
		friend class Node;

	private:
		Ref<DirectionalLightUD> m_UData;
		Ref<UniformBuffer> m_UBuffer;
	};

	template<>
	bool Node::is<LightNode>() const;

	template<>
	bool Node::is<DirectionalLightNode>() const;

	template<>
	Ref<UniformBuffer> Node::getRenderingData<DirectionalLightNode>() const;
}

#endif