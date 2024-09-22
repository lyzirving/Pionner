#include "SpriteNode.h"

#include "asset/AssetMgr.h"

#include "scene/Factory.h"
#include "scene/Components.h"

#include "gfx/renderer/RenderContext.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/MeshRenderBuffer.h"
#include "gfx/resource/SpriteMaterial.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SpriteNode"

namespace pio
{
	PIO_NODE_IMPL_CONSTRUCOR(SpriteNode, Node)

	SpriteNode::~SpriteNode() = default;

	void SpriteNode::update(Ref<RenderContext>& context)
	{
		auto* spriteRender = getComponent<SpriteRenderer>();
		auto* transComp = getComponent<TransformComponent>();

		auto p = parent();
		TransformComponent* parentTrans{ nullptr }; 		

		auto spriteMat = AssetMgr::GetRuntimeAsset<SpriteMaterial>(spriteRender->MatHnd);
		spriteMat->setColor(spriteRender->Color);
		spriteMat->setFlipX(spriteRender->FlipX);
		spriteMat->setFlipY(spriteRender->FlipY);
		spriteMat->update(context);

		auto buff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(spriteRender->BuffHnd);
		buff->Transform.setPosition(transComp->Position);
		buff->Transform.setEuler(transComp->Rotation);
		buff->Transform.setScale(transComp->Scale);
		if (p && (parentTrans = p->getComponent<TransformComponent>()))
		{
			buff->Transform.addTranslation(parentTrans->Position);
			buff->Transform.addEuler(parentTrans->Rotation);
			buff->Transform.addScale(parentTrans->Scale);
		}
		buff->update(context);
	}

	void SpriteNode::onInit()
	{
		auto context = m_context.lock();

		addComponent<SpriteRenderer, TransformComponent>();

		auto* transComp = getComponent<TransformComponent>();
		auto* spriteRender = getComponent<SpriteRenderer>();

		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->m_triangles = Factory::MakeSquare(2.f, 2.f);

		auto spriteMat = context->getMaterial(GpuAttr::Mat::SPRITE, true);
		spriteMat->as<SpriteMaterial>()->setSpriteTexture(context->getTexture(GpuAttr::Tex::DIST_LIGHT));
		spriteRender->MatHnd = spriteMat->assetHnd();
		spriteRender->MeshHnd = mesh->assetHnd();

		auto renderBuff = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>();
		renderBuff->setUp(context, mesh);
		spriteRender->BuffHnd = renderBuff->assetHnd();
	}

	template<>
	bool Node::is<SpriteNode>() const { return nodeType() == NodeType::Sprite; }
}