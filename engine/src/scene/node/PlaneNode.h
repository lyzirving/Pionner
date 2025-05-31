#pragma once

#include "StaticMeshNode.h"

namespace pio
{
	class PlaneNode : public StaticMeshNode
	{
		RTTR_ENABLE(StaticMeshNode)
	public:
		PlaneNode() : StaticMeshNode() {}
		virtual ~PlaneNode() = default;

	protected:
		virtual void OnInit() override;
	};
}