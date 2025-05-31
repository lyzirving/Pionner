#include "PhysicsWorld.h"
#include "PhysicsActor.h"

#include "scene/node/Node.h"

namespace pio
{
	PhysicsWorld::PhysicsWorld(const Ref<RenderContext>& context) : Object(), m_Context(context)
	{
	}

	Ref<PhysicsActor> PhysicsWorld::CreateActor(const Ref<Node>& node)
	{		
		if(!node)
			return Ref<PhysicsActor>();

		Ref<PhysicsActor> actor;
		auto it = m_Actors.find(node->GetName());
		if(it == m_Actors.end())
		{
			actor = CreateRef<PhysicsActor>(Self<PhysicsWorld>());
			actor->AttachNode(node);
			node->AttachActor(actor);
			actor->Init();
			m_Actors.insert({ node->GetName(), actor });
		}
		else
		{
			actor = m_Actors[node->GetName()];
		}
		return actor;
	}

	void PhysicsWorld::Tick(const Ref<RenderContext>& context)
	{
		for(auto& it : m_Actors)
		{
			it.second->Tick(context);
		}
	}
}