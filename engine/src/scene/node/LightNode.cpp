#include "LightNode.h"

#include "GlobalSettings.h"

#include "scene/Components.h"
#include "scene/3d/Camera.h"
#include "scene/node/CameraNode.h"

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
		m_shadowCam = CreateRef<Camera>();

		m_UData = CreateRef<DirectionalLightUD>();
		m_UDataShadow = CreateRef<DirectionalLightShadowDataUD>();

		m_UBuffer = UniformBuffer::Create(context, m_UData->Block.getByteUsed(), UBBinding_DirectionalLight, BufferUsage::Dynamic);
		m_UBufferShadow = UniformBuffer::Create(context, m_UDataShadow->Block.getByteUsed(), UBBinding_DirectionalLightShadow, BufferUsage::Dynamic);

		addComponent<DirectionalLightComponent, TransformComponent>();

		auto* lightComp = getComponent<DirectionalLightComponent>();
		auto* transComp = getComponent<TransformComponent>();

		lightComp->BuffId = m_UBuffer->assetHnd();
		transComp->Rotation = glm::vec3(-30.f, 0.f, 0.f);
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

	void DirectionalLightNode::update(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{
		update(context);

		//Update shadow data
		auto& camera = camNode->camera();
		m_shadowCam->setPrjType(ProjectionType_Orthographic);
		m_shadowCam->setPosition(camera->position());
		m_shadowCam->setEuler(camera->euler());
		m_shadowCam->setNear(camera->frustNear());
		m_shadowCam->setFar(camera->frustFar());
		m_shadowCam->setSize(5.f);
		m_shadowCam->setAspect(1.f);
		m_shadowCam->flush();

		m_UDataShadow->ViewMat = m_shadowCam->viewMat();
		m_UDataShadow->PrjMat = m_shadowCam->prjMat();
		m_UDataShadow->ShadowMapSize = GlobalSettings::ShadowResolution();
		m_UDataShadow->FrustumSize = m_shadowCam->size();
		m_UDataShadow->serialize();
		context->uploadData(m_UDataShadow->Block.getBuffer()->as<void*>(), m_UDataShadow->Block.getByteUsed(), m_UBufferShadow);
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