#pragma once

#include "Node.h"

namespace pio
{
	class LitNode : public Node
	{
		RTTR_ENABLE(Node)
	public:
		LitNode() : Node() {}
		virtual ~LitNode() = default;
	};
}