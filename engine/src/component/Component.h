#pragma once

#include "Common.h"

namespace pio
{
	class Node;
	class RenderContext;

	class Component : public Object
	{
		RTTR_ENABLE(Object)
	public:
		Component() {};
		virtual ~Component() = default;
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnTick() {}

	public:
		template<typename T>
		static Ref<T> NewComponent(const Ref<RenderContext>& context)
		{
			Ref<Component> comp = CreateRef<T>();
			comp->m_Context = context;
			comp->OnAttach();
			return RefCast<Component, T>(comp);
		}

	protected:
		bool HaveParentHost() const;

	protected:
		WeakRef<Node> m_Host;
		WeakRef<RenderContext> m_Context;

	private:
		friend class Node;
	};
}