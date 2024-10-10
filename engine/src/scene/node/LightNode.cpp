#include "LightNode.h"
#include "SpriteNode.h"

#include "GlobalSettings.h"

#include "asset/AssetMgr.h"

#include "scene/Factory.h"
#include "scene/Components.h"

#include "scene/3d/ShadowMap.h"
#include "scene/3d/CascadeShadowMap.h"

#include "scene/node/CameraNode.h"
#include "scene/node/SpriteNode.h"
#include "scene/node/GizmoNode.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/LightData.h"
#include "gfx/resource/MeshRenderBuffer.h"
#include "gfx/resource/material/SpriteMaterial.h"
#include "gfx/renderer/RenderContext.h"

#include "gfx/rhi/Texture.h"
#include "gfx/rhi/UniformBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LightNode"

namespace pio
{
	PIO_NODE_IMPL_CONSTRUCOR(LightNode, MovableNode)

	LightNode::~LightNode() = default;

	PIO_NODE_IMPL_CONSTRUCOR(DirectionalLightNode, LightNode)

	DirectionalLightNode::~DirectionalLightNode() = default;

	void DirectionalLightNode::onInit()
	{
		auto context = m_context.lock();

		m_UData = CreateRef<DirectionalLightUD>();		
		m_UBuffer = UniformBuffer::Create(context, m_UData->Block.getByteUsed(), UBBinding_DirectionalLight, BufferUsage::Dynamic);

		addComponent<DirectionalLightComponent>();

		auto* lightComp = getComponent<DirectionalLightComponent>();		
		lightComp->BuffId = m_UBuffer->assetHnd();
	}

	void DirectionalLightNode::onAttach(Ref<Scene>& scene)
	{
		auto context = m_context.lock();

		{
			auto sprite = addChild<SpriteNode>("LitSprite");
			auto* spriteRender = sprite->getComponent<SpriteRenderer>();
			spriteRender->BillBoard = true;

			auto meshData = Factory::MakeSquare(1.f, 1.f);
			auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
			mesh->setData(meshData);

			auto spriteMat = context->getMaterial(GpuAttr::Mat::SPRITE, true);
			spriteMat->as<SpriteMaterial>()->setSpriteTexture(context->getTexture(GpuAttr::Tex::DIST_LIGHT));
			spriteRender->MatHnd = spriteMat->assetHnd();
			spriteRender->MeshHnd = mesh->assetHnd();

			auto renderBuff = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>();
			renderBuff->setUp(context, meshData->getVertice(), meshData->getIndice());
			spriteRender->BuffHnd = renderBuff->assetHnd();
		}

		{
			m_gizmo = RefCast<Node, GizmoNode>(addChild<DirLightGizmo>("DirLightGizmo"));
		}
	}

	void DirectionalLightNode::onUpdate(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingData& renderingData)
	{
		updateLight(context, camNode);
		updateShadow(context, camNode);

		renderingData.UnimBuffSet.insert({ m_UBuffer->binding(), m_UBuffer->assetHnd() });
	}

	const glm::vec3& DirectionalLightNode::direction() const
	{
		return m_UData->Direction;
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
		switch (GlobalSettings::RenderConfig.LightingTech)
		{
			case LightTech::ShadowMap:
			{
				auto* sdMap = context->getLightTech(LightTech::ShadowMap)->as<ShadowMap>();
				sdMap->update(context, camNode, self<DirectionalLightNode>());
				break;
			}
			case LightTech::CascadeShadowMap:
			{
				auto* cascadeSdMap = context->getLightTech(LightTech::CascadeShadowMap)->as<CascadeShadowMap>();
				cascadeSdMap->update(context, camNode, self<DirectionalLightNode>());
				break;
			}
			default:
				break;
		}
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
}