#include "SpriteNode.h"

#include "scene/Components.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SpriteNode"

namespace pio
{
	SpriteNode::SpriteNode(Ref<RenderContext>& context, const entt::entity& key, entt::registry& regi, const std::string& name)
		: Node(context, key, regi, name)
	{
		addComponent<SpriteRenderer, TransformComponent>();
	}

	SpriteNode::~SpriteNode() = default;

	void SpriteNode::update(Ref<RenderContext>& context)
	{
	}

	template<>
	bool Node::is<SpriteNode>() const { return nodeType() == NodeType::Sprite; }
}