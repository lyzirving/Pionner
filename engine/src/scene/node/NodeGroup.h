#pragma once

#include "Node.h"

namespace pio
{
	class NodeGroup : public Node
	{
		RTTR_ENABLE(Node)
	public:
		NodeGroup() : Node() {}
		virtual ~NodeGroup() = default;

		bool GroupInsert(const Ref<Node>& node);
		bool GroupRemoval(const Ref<Node>& node);

		Ref<Node> PopFront();
		int32_t Size() const { return m_Children.size(); }

	protected:
		virtual void OnGroupSizeChanged() {};
		virtual void OnTickChildren(const Ref<RenderContext>& context) override { /*should override and do nothing*/}
		virtual void OnRenderChildren(const Ref<RenderContext>& context) override { /*should override and do nothing*/}
	};
}