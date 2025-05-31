#include "CameraNode.h"

#include "GlobalSettings.h"

#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/renderer/RenderContext.h"

#include "component/TransformComponent.h"
#include "component/CameraComponent.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CameraNode"

namespace pio
{
	CameraNode::CameraNode() : Node()
	{
	}

	void CameraNode::OnInit()
	{
		CreateRenderTarget();
		AddComponent<CameraComponent>();
	}

	void CameraNode::SetAspect(float aspect)
	{
		GetComponent<CameraComponent>()->SetAspect(aspect);
	}

	void CameraNode::SetNear(float near)
	{
		GetComponent<CameraComponent>()->SetNear(near);
	}

	void CameraNode::SetFar(float far)
	{
		GetComponent<CameraComponent>()->SetFar(far);
	}

	void CameraNode::SetProjectionType(ProjectionType type)
	{
		GetComponent<CameraComponent>()->SetPrjType(type);
	}

	void CameraNode::SetFov(float fov)
	{
		GetComponent<CameraComponent>()->SetFov(fov);
	}

	void CameraNode::SetOrthoSize(float size)
	{
		GetComponent<CameraComponent>()->SetSize(size);
	}

	float CameraNode::GetAspect() const
	{
		return GetComponent<CameraComponent>()->GetAspect();
	}

	float CameraNode::GetNear() const
	{
		return GetComponent<CameraComponent>()->GetNear();
	}

	float CameraNode::GetFar() const
	{
		return GetComponent<CameraComponent>()->GetFar();
	}

	ProjectionType CameraNode::GetProjectionType() const
	{
		return GetComponent<CameraComponent>()->GetPrjType();
	}

	float CameraNode::GetFov() const
	{
		return GetComponent<CameraComponent>()->GetFov();
	}

	float CameraNode::GetOrthoSize() const
	{
		return GetComponent<CameraComponent>()->GetSize();
	}

	void CameraNode::OnTickComponents(const Ref<RenderContext>& context)
	{
		auto transComp = GetComponent<TransformComponent>();
		auto camComp = GetComponent<CameraComponent>();

		transComp->OnTick();

		camComp->SetPosition(transComp->GetPosition());
		camComp->SetRotation(transComp->GetRotation());

		camComp->OnTick();
	}

	void CameraNode::OnRender(const Ref<RenderContext>& context)
	{
		auto& data = context->GetRenderingData();
		auto buffer = GetComponent<CameraComponent>()->GetUBuffer();
		data.UBuffs[buffer->Binding()] = buffer;
	}

	void CameraNode::CreateRenderTarget()
	{
		auto context = m_Context.lock();
		auto colorSize = GlobalSettings::ColorResolution();
		auto depthSize = GlobalSettings::ShadowResolution();

		FrameBufferSpecific fboSpec;
		fboSpec.Name = std::string(m_Name).append("_RenderTarget");
		fboSpec.Width = colorSize.x;
		fboSpec.Height = colorSize.y;
		PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

		TextureSpecificBuilder colorBufferSpec;
		colorBufferSpec.Name(std::string(fboSpec.Name).append("_ColorBuffer"))
			.Type(TextureType::TwoDimen)
			.Format(TextureFormat::RGBA_HALF)
			.Width(colorSize.x).Height(colorSize.y)
			.TexWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
			.TexFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

		fboSpec.ColorSpec.push_back(colorBufferSpec.Build());
		m_RenderTarget = FrameBuffer::Create(context, fboSpec);
		context->UploadData(m_RenderTarget);
	}
}