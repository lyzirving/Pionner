#pragma once

#include "gfx/GfxDef.h"
#include "scene/node/Node.h"

namespace pio
{
	class FrameBuffer;

	class CameraNode : public Node
	{
		RTTR_ENABLE(Node)
	public:
		CameraNode();
		virtual ~CameraNode() = default;		

		void SetAspect(float aspect);
		void SetNear(float near);
		void SetFar(float far);
		void SetProjectionType(ProjectionType type);
		void SetFov(float fov);
		void SetOrthoSize(float size);

		float GetAspect() const;
		float GetNear() const;
		float GetFar() const;
		ProjectionType GetProjectionType() const;
		float GetFov() const;
		float GetOrthoSize() const;

		const Ref<FrameBuffer>& GetTarget() const { return m_RenderTarget; }

	protected:
		virtual void OnInit() override;
		virtual void OnTickComponents(const Ref<RenderContext>& context) override;
		virtual void OnRender(const Ref<RenderContext>& context) override;

		void CreateRenderTarget();

	private:
		Ref<FrameBuffer> m_RenderTarget;
	};
}