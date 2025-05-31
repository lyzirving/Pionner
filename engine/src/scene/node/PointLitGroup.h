#pragma once

#include "NodeGroup.h"

namespace pio
{
	class PointLitGroup : public NodeGroup
	{
		RTTR_ENABLE(NodeGroup)
	public:
		PointLitGroup() : NodeGroup() {}
		virtual ~PointLitGroup() = default;

	protected:
		virtual void OnInit() override;
		virtual void OnRender(const Ref<RenderContext>& context) override;			
		virtual void OnGroupSizeChanged() override;
	};
}