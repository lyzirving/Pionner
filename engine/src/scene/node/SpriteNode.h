#ifndef __PIONNER_SCENE_NODE_SPRITE_NODE_H__
#define __PIONNER_SCENE_NODE_SPRITE_NODE_H__

#include "Node.h"
#include "gfx/resource/RenderingData.h"

namespace pio
{
	class SpriteNode : public Node
	{
		PIO_NODE_DECLARE(SpriteNode, NodeType::Sprite)
	public:
		~SpriteNode();		
		virtual void onInit() override;
		virtual void onUpdate(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingData& renderingData) override;

	protected:
		MeshRenderingItem onUpdateInner(Ref<RenderContext>& context, Ref<CameraNode>& camNode);
	};

	template<>
	bool Node::is<SpriteNode>() const;
}

#endif