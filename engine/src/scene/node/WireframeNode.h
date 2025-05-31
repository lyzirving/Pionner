#pragma once

#include "StaticMeshNode.h"

namespace pio
{
	class WireframeNode : public StaticMeshNode
	{
		RTTR_ENABLE(StaticMeshNode)
	public:
		WireframeNode() : StaticMeshNode() {}
		virtual ~WireframeNode() = default;

		void SetColor(const glm::vec3& color);

	protected:
		virtual void OnInit() override;
		virtual void OnTick(const Ref<RenderContext>& context) override { /* should override and do nothing */ }
	};
}