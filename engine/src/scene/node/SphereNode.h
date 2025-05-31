#pragma once

#include "StaticMeshNode.h"

namespace pio
{
	class SphereNode : public StaticMeshNode
	{
		RTTR_ENABLE(StaticMeshNode)
	public:
		SphereNode() : StaticMeshNode() {}
		virtual ~SphereNode() = default;

	protected:
		virtual void OnInit() override;
	};
}