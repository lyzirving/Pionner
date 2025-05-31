#pragma once

#include "LitNode.h"
#include "gfx/GfxDef.h"

namespace pio
{
	class SpriteNode;
	class WireframeNode;

	class PointLitNode : public LitNode
	{
		RTTR_ENABLE(LitNode)
	public:
		PointLitNode() : LitNode() {}
		virtual ~PointLitNode() = default;
		virtual void OnSelect() override;
		virtual void OnDeselect() override;

		void SetColor(const glm::vec3& color);
		void SetIntensity(float intensity);
		void SetRadius(float radius);
		void SetFalloff(float falloff);
		void SetShadowMode(ShadowMode mode);
		void SetShadowIntensity(float intensity);
		void SetShadowBias(float bias);

		int32_t GetIndex() const;
		const glm::vec3& GetColor() const;
		float GetIntensity() const;
		float GetRadius() const;
		float GetFalloff() const;
		bool IsCastShadow() const;
		ShadowMode GetShadowMode() const;
		float GetShadowIntensity() const;
		float GetShadowBias() const;

	protected:
		virtual void OnInit() override;
		virtual void OnTickComponents(const Ref<RenderContext>& context) override;
		virtual void OnGroupIndexChange() override;

	private:
		Ref<SpriteNode> m_Sprite;
		Ref<WireframeNode> m_Gizmo;
	};
}