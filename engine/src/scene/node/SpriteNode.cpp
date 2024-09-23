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

		auto p = getParent();
		TransformComponent* parentTrans{ nullptr }; 		

		auto spriteMat = AssetMgr::GetRuntimeAsset<SpriteMaterial>(spriteRender->MatHnd);
		auto buff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(spriteRender->BuffHnd);
		if (!spriteMat || !buff || !buff->valid())
		{
			LOGW("node has not been initialized");
			return;
		}
		spriteMat->setColor(spriteRender->Color);
		spriteMat->setFlipX(spriteRender->FlipX);
		spriteMat->setFlipY(spriteRender->FlipY);
		spriteMat->update(context);

		buff->Transform.setPosition(transComp->Position);
		buff->Transform.setEuler(spriteMat->renderingMode() == RenderingMode_Overlay ? glm::vec3(0.f) : transComp->Rotation);
		buff->Transform.setScale(transComp->Scale);
		if (p && (parentTrans = p->getComponent<TransformComponent>()))
		{
			buff->Transform.addTranslation(parentTrans->Position);
			buff->Transform.addEuler(spriteMat->renderingMode() == RenderingMode_Overlay ? glm::vec3(0.f) : parentTrans->Rotation);
			buff->Transform.addScale(parentTrans->Scale);
		}
		buff->update(context);
	}

	void SpriteNode::onInit()
	{
		addComponent<SpriteRenderer, TransformComponent>();		
	}

	template<>
	bool Node::is<SpriteNode>() const { return nodeType() == NodeType::Sprite; }

	template<>
	MeshRenderingItem Node::getRenderingData<SpriteNode>() const
	{
		MeshRenderingItem item;
		if (is<SpriteNode>())
		{			
			auto* render = getComponent<SpriteRenderer>();
			auto material = AssetMgr::GetRuntimeAsset<SpriteMaterial>(render->MatHnd);
			if (material)
			{
				item.Mode = material->renderingMode();
				item.RenderBuffFilter = render->BuffHnd;
				item.MaterialFilter = render->MatHnd;
			}			
		}
		return item;
	}
}