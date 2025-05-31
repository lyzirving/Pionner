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

	protected:
		bool HaveParentHost() const;

	protected:
		WeakRef<Node> m_Host;
		WeakRef<RenderContext> m_Context;

	private:
		friend class Node;
	};
}