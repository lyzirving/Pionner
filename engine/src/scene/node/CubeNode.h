#pragma once

#include "StaticMeshNode.h"

namespace pio
{
	class CubeNode : public StaticMeshNode
	{
		RTTR_ENABLE(StaticMeshNode)
	public:
		CubeNode() : StaticMeshNode() {}
		virtual ~CubeNode() = default;

	protected:
		virtual void OnInit() override;
	};
}