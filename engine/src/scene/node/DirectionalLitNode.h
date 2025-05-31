#pragma once

#include "LitNode.h"
#include "gfx/GfxDef.h"

namespace pio
{
	class WireframeNode;

	class DirectionalLitNode : public LitNode
	{
		RTTR_ENABLE(LitNode)
	public:
		DirectionalLitNode() : LitNode() {}
		virtual ~DirectionalLitNode() = default;

		virtual void OnSelect() override;
		virtual void OnDeselect() override;

		void SetColor(const glm::vec3& color);
		void SetIntensity(float intensity);
		void SetShadowMode(ShadowMode mode);
		void SetShadowIntensity(float intensity);
		void SetShadowBias(float bias);

		const glm::vec3& GetColor() const;
		float GetIntensity() const;
		bool IsCastShadow() const;
		ShadowMode GetShadowMode() const;
		float GetShadowIntensity() const;
		float GetShadowBias() const;

	protected:
		virtual void OnInit() override;
		virtual void OnTickComponents(const Ref<RenderContext>& context) override;
		virtual void OnRender(const Ref<RenderContext>& context) override;

	private:
		Ref<WireframeNode> m_Gizmo;
	};
}