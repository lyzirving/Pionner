#include "MovableNode.h"

#include "scene/Components.h"

namespace pio
{
	PIO_NODE_IMPL_CONSTRUCOR(MovableNode, Node)

	MovableNode::~MovableNode() = default;

	void MovableNode::onInit()
	{
		addComponent<TransformComponent>();
	}

	void MovableNode::addTranslation(const glm::vec3& delta)
	{
		auto* comp = getComponent<TransformComponent>();
		comp->Position += delta;
	}

	void MovableNode::addRotation(const glm::vec3& delta)
	{
		auto* comp = getComponent<TransformComponent>();
		comp->Rotation += delta;
	}

	void MovableNode::setPosition(const glm::vec3& pos)
	{
		auto* comp = getComponent<TransformComponent>();
		comp->Position = pos;
	}

	void MovableNode::setRotation(const glm::vec3& euler)
	{
		auto* comp = getComponent<TransformComponent>();
		comp->Rotation = euler;
	}

	void MovableNode::setScale(const glm::vec3& scale)
	{
		auto* comp = getComponent<TransformComponent>();
		comp->Scale = scale;
	}
}