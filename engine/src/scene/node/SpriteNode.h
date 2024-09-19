#ifndef __PIONNER_SCENE_NODE_SPRITE_NODE_H__
#define __PIONNER_SCENE_NODE_SPRITE_NODE_H__

#include "Node.h"

namespace pio
{
	class SpriteNode : public Node
	{
		PIO_DEFINE_NODE_TYPE(NodeType::Sprite)
	public:
		SpriteNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name);
		~SpriteNode();

		virtual void update(Ref<RenderContext>& context) override;
	};

	template<>
	bool Node::is<SpriteNode>() const;
}

#endif