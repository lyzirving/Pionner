#include "SpriteNode.h"

#include "asset/AssetMgr.h"

#include "scene/Factory.h"
#include "scene/Components.h"
#include "scene/node/CameraNode.h"
#include "scene/3d/Camera.h"

#include "gfx/renderer/RenderContext.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/MeshRenderBuffer.h"
#include "gfx/resource/material/SpriteMaterial.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SpriteNode"

namespace pio
{
	PIO_NODE_IMPL_CONSTRUCOR(SpriteNode, MovableNode)

	SpriteNode::~SpriteNode() = default;

	void SpriteNode::onInit()
	{
		addComponent<SpriteRenderer>();
	}

	void SpriteNode::onUpdate(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingData& renderingData)
	{
		auto item = onUpdateInner(context, camNode);
		renderingData.submitSprite(std::move(item));
	}

	MeshRenderingItem SpriteNode::onUpdateInner(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{
		MeshRenderingItem item;

		auto* spriteRender = getComponent<SpriteRenderer>();
		auto* transComp = getComponent<TransformComponent>();

		auto p = getParent();
		TransformComponent* parentTrans{ nullptr };

		auto spriteMat = AssetMgr::GetRuntimeAsset<SpriteMaterial>(spriteRender->MatHnd);
		auto buff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(spriteRender->BuffHnd);
		if (!spriteMat || !buff || !buff->valid())
		{
			LOGW("node has not been initialized");
			return item;
		}
		spriteMat->setColor(spriteRender->Color);
		spriteMat->setFlipX(spriteRender->FlipX);
		spriteMat->setFlipY(spriteRender->FlipY);
		spriteMat->onUpdate(context);

		buff->Transform.setPosition(transComp->Position);
		buff->Transform.setEuler(spriteRender->BillBoard ? Camera::BillBoardRotate(camNode) : transComp->Rotation);
		buff->Transform.setScale(transComp->Scale);
		if (p && (parentTrans = p->getComponent<TransformComponent>()))
		{
			buff->Transform.addTranslation(parentTrans->Position);
			buff->Transform.addEuler(spriteRender->BillBoard ? glm::vec3(0.f) : parentTrans->Rotation);
			buff->Transform.addScale(parentTrans->Scale);
		}
		buff->onUpdate(context);

		item.Mode = spriteMat->renderingMode();
		item.RenderBuffFilter = spriteRender->BuffHnd;
		item.MaterialFilter = spriteRender->MatHnd;
		return item;
	}

	template<>
	bool Node::is<SpriteNode>() const { return nodeType() == NodeType::Sprite; }
}