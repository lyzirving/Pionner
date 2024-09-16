#include "LightNode.h"

#include "scene/Components.h"

#include "gfx/resource/Light.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LightNode"

namespace pio
{
	LightNode::LightNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name) 
		: Node(context, key, regi, name)
	{
	}

	LightNode::~LightNode() = default;

	DirectionalLightNode::DirectionalLightNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name)
		: LightNode(context, key, regi, name)
	{
		m_UData = CreateRef<DirectionalLightUD>();
		m_UBuffer = UniformBuffer::Create(context, m_UData->Block.getByteUsed(), UBBinding_DirectionalLight, BufferUsage::Dynamic);
		addComponent<DirectionalLightComponent, TransformComponent>();

		auto* lightComp = getComponent<DirectionalLightComponent>();
		lightComp->BuffId = m_UBuffer->assetHnd();
	}

	DirectionalLightNode::~DirectionalLightNode() = default;

	void DirectionalLightNode::update(Ref<RenderContext>& context)
	{
		auto* lightComp = getComponent<DirectionalLightComponent>();
		auto* transComp = getComponent<TransformComponent>();

		m_UData->Color = lightComp->Color;
		m_UData->Intensity = lightComp->Intensity;
		m_UData->Bias = lightComp->Bias;
		m_UData->NormalBias = lightComp->NormalBias;
		m_UData->ShadowMode = lightComp->ShadowMode;
		m_UData->CastShadow = lightComp->CastShadow;
		m_UData->ShadowIntensity = lightComp->ShadowIntensity;

		Transform3D transform;
		transform.setEuler(transComp->Rotation);
		m_UData->Direction = transform.rotMat() * glm::vec4(-World::Forward, 0.f);
		m_UData->Direction = glm::normalize(m_UData->Direction);

		m_UData->serialize();
		context->uploadData(m_UData->Block.getBuffer()->as<void*>(), m_UData->Block.getByteUsed(), m_UBuffer);
	}

	template<>
	bool Node::is<LightNode>() const { return nodeType() == NodeType::Light; }

	template<>
	bool Node::is<DirectionalLightNode>() const
	{
		if (is<LightNode>())
		{
			return as<LightNode>()->lightType() == LightType::DirectionLight;
		}
		return false;
	}

	template<>
	Ref<UniformBuffer> Node::getRenderingData<DirectionalLightNode>() const
	{
		if (is<DirectionalLightNode>())
		{
			return as<DirectionalLightNode>()->m_UBuffer;
		}
		return Ref<UniformBuffer>();
	}
}