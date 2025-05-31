#pragma once

#include "Common.h"

namespace pio
{
	class Node;
	class PhysicsWorld;
	class RenderContext;

	class PhysicsActor : public Object
	{
		RTTR_ENABLE(Object)
	public:
		PhysicsActor(const Ref<PhysicsWorld>& world);
		virtual ~PhysicsActor() = default;
		virtual void OnInit() {}

		void AttachNode(const Ref<Node>& node);
		void Init();
		void Tick(const Ref<RenderContext>& context);

		Ref<Node> GetNode() { return m_Node.lock(); }
		Ref<PhysicsWorld> GetPhysicsWorld() { return m_World.lock(); }

	protected:
		WeakRef<Node> m_Node;
		WeakRef<PhysicsWorld> m_World;
	};
}