#pragma once

#include "Common.h"

namespace pio
{
	class Node;
	class PhysicsActor;
	class RenderContext;

	class PhysicsWorld : public Object
	{
		RTTR_ENABLE(Object)
	public:
		PhysicsWorld(const Ref<RenderContext>& context);
		virtual ~PhysicsWorld() = default;

		Ref<PhysicsActor> CreateActor(const Ref<Node>& node);
		void Tick(const Ref<RenderContext>& context);

	private:
		WeakRef<RenderContext> m_Context;
		std::unordered_map<std::string, Ref<PhysicsActor>> m_Actors;
	};
}