#include "LightNode.h"
#include "SpriteNode.h"

#include "GlobalSettings.h"

#include "asset/AssetMgr.h"

#include "scene/Factory.h"
#include "scene/Components.h"
#include "scene/3d/Camera.h"

#include "scene/node/CameraNode.h"
#include "scene/node/SpriteNode.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/resource/Light.h"
#include "gfx/resource/Mesh.h"
#include "gfx/resource/MeshRenderBuffer.h"
#include "gfx/resource/SpriteMaterial.h"

#include "gfx/rhi/Texture.h"
#include "gfx/rhi/UniformBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LightNode"

namespace pio
{
	PIO_NODE_IMPL_CONSTRUCOR(LightNode, Node)

	LightNode::~LightNode() = default;

	PIO_NODE_IMPL_CONSTRUCOR(DirectionalLightNode, LightNode)

	DirectionalLightNode::~DirectionalLightNode() = default;

	void DirectionalLightNode::update(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{
		updateLight(context, camNode);
		updateShadow(context, camNode);
	}

	void DirectionalLightNode::onInit()
	{
		auto context = m_context.lock();

		m_shadowCam = CreateRef<Camera>();

		m_UData = CreateRef<DirectionalLightUD>();
		m_UDataShadow = CreateRef<DirectionalLightShadowDataUD>();

		m_UBuffer = UniformBuffer::Create(context, m_UData->Block.getByteUsed(), UBBinding_DirectionalLight, BufferUsage::Dynamic);
		m_UBufferShadow = UniformBuffer::Create(context, m_UDataShadow->Block.getByteUsed(), UBBinding_DirectionalLightShadow, BufferUsage::Dynamic);

		addComponent<DirectionalLightComponent, TransformComponent>();

		auto* lightComp = getComponent<DirectionalLightComponent>();
		auto* transComp = getComponent<TransformComponent>();

		lightComp->BuffId = m_UBuffer->assetHnd();

		transComp->Position = glm::vec3(-4.f, 4.f, 4.f);
		transComp->Rotation = glm::vec3(-30.f, -45.f, 0.f);
	}

	void DirectionalLightNode::onAttach(Ref<Scene>& scene)
	{
		auto context = m_context.lock();

		auto sprite = addChild<SpriteNode>("LitSprite");
		auto* transComp = sprite->getComponent<TransformComponent>();
		auto* spriteRender = sprite->getComponent<SpriteRenderer>();

		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->m_triangles = Factory::MakeSquare(1.f, 1.f);

		auto spriteMat = context->getMaterial(GpuAttr::Mat::SPRITE, true);
		spriteMat->as<SpriteMaterial>()->setSpriteTexture(context->getTexture(GpuAttr::Tex::DIST_LIGHT));
		spriteRender->MatHnd = spriteMat->assetHnd();
		spriteRender->MeshHnd = mesh->assetHnd();

		auto renderBuff = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>();
		renderBuff->setUp(context, mesh);
		spriteRender->BuffHnd = renderBuff->assetHnd();
	}

	void DirectionalLightNode::updateLight(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{
		auto* lightComp = getComponent<DirectionalLightComponent>();
		auto* transComp = getComponent<TransformComponent>();
		auto* spriteRender = getComponent<SpriteRenderer>();

		m_UData->Color = lightComp->Color;
		m_UData->Intensity = lightComp->Intensity;
		m_UData->Bias = lightComp->Bias;
		m_UData->NormalBias = lightComp->NormalBias;
		m_UData->ShadowMode = lightComp->ShadowMode;
		m_UData->CastShadow = lightComp->ShadowMode != ShadowMode_None;
		m_UData->ShadowIntensity = lightComp->ShadowIntensity;

		Transform3D transform;
		transform.setEuler(transComp->Rotation);
		m_UData->Direction = transform.rotMat() * glm::vec4(-World::Forward, 0.f);
		m_UData->Direction = glm::normalize(m_UData->Direction);
		m_UData->serialize();

		context->uploadData(m_UData->Block.getBuffer()->as<void*>(), m_UData->Block.getByteUsed(), m_UBuffer);
	}

	void DirectionalLightNode::updateShadow(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{
		//Update shadow data
		auto camera = camNode->camera();
		auto* lightComp = getComponent<DirectionalLightComponent>();
		auto* transComp = getComponent<TransformComponent>();

		Transform3D transform;
		transform.setEuler(transComp->Rotation);

		m_shadowCam->setPosition(transComp->Position);
		m_shadowCam->setEuler(transComp->Rotation);
		m_shadowCam->setNear(camera->frustNear());
		m_shadowCam->setFar(camera->frustFar());
		m_shadowCam->setSize(5.f);
		m_shadowCam->setAspect(1.f);
		m_shadowCam->flush();

		auto shadowSize = GlobalSettings::ShadowResolution().x;
		m_UDataShadow->ViewMat = m_shadowCam->viewMat();
		m_UDataShadow->PrjMat = m_shadowCam->orthoMat();
		m_UDataShadow->ShadowMapSize = glm::vec2(shadowSize);
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