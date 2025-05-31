#pragma once

#include "SphereNode.h"
#include "gfx/rhi/RenderStateAttrs.h"

namespace pio
{
	class Material;
	class FrameBuffer;

	class MaterialBall : public SphereNode
	{
		RTTR_ENABLE(SphereNode)
	public:
		MaterialBall();
		virtual ~MaterialBall() = default;

		void SetMaterial(const Ref<Material>& material);
		void OnDraw();

		float GetAspect() const;
		float GetColorBufferId() const;

	protected:
		virtual void OnInit() override;	

	private:		
		Ref<Material> m_Material;
		Ref<FrameBuffer> m_FrameBuff;
		RenderStateAttrs m_Attrs;
	};
}