#ifndef __PIONNER_SCENE_NODE_SPRITE_NODE_H__
#define __PIONNER_SCENE_NODE_SPRITE_NODE_H__

#include "Node.h"

namespace pio
{
	class SpriteNode : public Node
	{
		PIO_NODE_DECLARE(SpriteNode, NodeType::Sprite)
	public:
		~SpriteNode();
		virtual void update(Ref<RenderContext>& context) override;
		virtual void onInit() override;
	};

	template<>
	bool Node::is<SpriteNode>() const;
}

#endif