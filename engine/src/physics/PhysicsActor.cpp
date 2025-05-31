#include "PhysicsActor.h"

#include "scene/node/Node.h"

namespace pio
{
	PhysicsActor::PhysicsActor(const Ref<PhysicsWorld>& world) : Object(), m_World(world)
	{
	}

	void PhysicsActor::AttachNode(const Ref<Node>& node)
	{
		m_Node = node;
	}

	void PhysicsActor::Init()
	{
		OnInit();
	}

	void PhysicsActor::Tick(const Ref<RenderContext>& context)
	{
	}
}