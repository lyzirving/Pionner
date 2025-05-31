#include "CameraComponent.h"

#include "gfx/rhi/UniformBlock.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CameraComponent"

namespace pio
{
	CameraComponent::CameraComponent() : Component()
	{		
	}

	void CameraComponent::OnAttach()
	{
		CreateUBuffer();
		OnPositionChange();
		OnRotationChange();
		OnPrjTypeChange();
		OnNearChange();
		OnFarChange();
		OnAspectChange();
		OnFovChange();
		OnSizeChange();
	}

	void CameraComponent::OnTick()
	{
		CalcCameraPose();
		CalcPersFrustum();
		CalcOrthoFrustum();
		Flush();
	}

	void CameraComponent::CreateUBuffer()
	{
		m_UBlock = CreateRef<UniformBlock>();
		m_UBlock->PushBack("ViewMat", UniformBlock::CreateData(UniformType::Mat4, "ViewMat"));
		m_UBlock->PushBack("PrjMat", UniformBlock::CreateData(UniformType::Mat4, "PrjMat"));
		m_UBlock->PushBack("OrthoMat", UniformBlock::CreateData(UniformType::Mat4, "OrthoMat"));
		m_UBlock->PushBack("Position", UniformBlock::CreateData(UniformType::Vec3, "Position"));
		m_UBlock->PushBack("FrustumFar", UniformBlock::CreateData(UniformType::Float, "FrustumFar"));
		m_UBlock->PushBack("PrjType", UniformBlock::CreateData(UniformType::Int, "PrjType"));
		m_UBlock->Calculate();

		m_UBuffer = UniformBuffer::Create(m_Context.lock(), m_UBlock->GetByteUsed(),
										  UBBinding_Camera, BufferUsage::Dynamic);
	}

	void CameraComponent::CalcCameraPose()
	{
		if(m_AttrsChange.test(CameraAttrBits_Pose))
		{
			Math::CalcCameraPose(m_ViewDir, m_CamRight, m_CamUp, m_CamFront);
			m_ViewMat = Math::CameraViewMat(m_Position, m_ViewDir, m_CamUp);			
			m_AttrsChange.reset(CameraAttrBits_Pose);

			auto layout = (*m_UBlock)["ViewMat"];
			m_UBlock->WriteAt(glm::value_ptr(m_ViewMat), sizeof(glm::mat4), layout->GetAlignOffset());			
		}		
	}

	void CameraComponent::CalcPersFrustum()
	{
		if(m_PrjType == ProjectionType_Perspective &&
		   (m_AttrsChange.test(CameraAttrBits_Frustum) ||
			m_AttrsChange.test(CameraAttrBits_Pers)))
		{
			m_PersMat = Math::PerspectiveMat(GetFov(), GetAspect(), GetNear(), GetFar());
			m_OrthoMat = glm::mat4(1.f);
			m_AttrsChange.reset(CameraAttrBits_Frustum);
			m_AttrsChange.reset(CameraAttrBits_Pers);

			auto layout = (*m_UBlock)["PrjMat"];
			m_UBlock->WriteAt(glm::value_ptr(m_PersMat), sizeof(glm::mat4), layout->GetAlignOffset());
		}
	}

	void CameraComponent::CalcOrthoFrustum()
	{
		if(m_PrjType == ProjectionType_Orthographic &&
		   (m_AttrsChange.test(CameraAttrBits_Frustum) ||
			m_AttrsChange.test(CameraAttrBits_Ortho)))
		{
			m_OrthoMat = Math::OrthogonalMat(FrustLeft(), FrustRight(), FrustBottom(), FrustTop(), GetNear(), GetFar());
			m_PersMat = glm::mat4(1.f);
			m_AttrsChange.reset(CameraAttrBits_Frustum);
			m_AttrsChange.reset(CameraAttrBits_Ortho);

			auto layout = (*m_UBlock)["OrthoMat"];
			m_UBlock->WriteAt(glm::value_ptr(m_OrthoMat), sizeof(glm::mat4), layout->GetAlignOffset());
		}
	}

	void CameraComponent::Flush()
	{
		if(m_UBlock->AnyChange())
		{
			m_Context.lock()->UploadData(m_UBlock->GetBuffer()->As<void*>(), m_UBlock->GetByteUsed(), m_UBuffer);
			m_UBlock->Invalidate(false);
		}
	}

	void CameraComponent::OnPositionChange()
	{
		m_AttrsChange.set(CameraAttrBits_Pose);
		auto layout = (*m_UBlock)["Position"];
		m_UBlock->WriteAt(glm::value_ptr(m_Position), sizeof(glm::vec3), layout->GetAlignOffset());
	}

	void CameraComponent::OnRotationChange()
	{
		m_AttrsChange.set(CameraAttrBits_Pose);		
		m_ViewDir = m_Rotation.Mat() * glm::vec4(-World::Forward, 0.f);
		m_ViewDir = glm::normalize(m_ViewDir);
	}

	void CameraComponent::OnPrjTypeChange()
	{
		if(m_PrjType == ProjectionType_Perspective)
		{
			m_AttrsChange.set(CameraAttrBits_Pers);
		}
		else
		{
			m_AttrsChange.set(CameraAttrBits_Ortho);
		}

		auto layout = (*m_UBlock)["PrjType"];
		int32_t val = m_PrjType;
		m_UBlock->WriteAt(&val, sizeof(int32_t), layout->GetAlignOffset());
	}

	void CameraComponent::OnNearChange()
	{
		m_AttrsChange.set(CameraAttrBits_Frustum);		
	}

	void CameraComponent::OnFarChange()
	{
		m_AttrsChange.set(CameraAttrBits_Frustum);
		auto layout = (*m_UBlock)["FrustumFar"];
		m_UBlock->WriteAt(&m_Far, sizeof(float), layout->GetAlignOffset());
	}

	void CameraComponent::OnAspectChange()
	{
		m_AttrsChange.set(CameraAttrBits_Frustum);
	}

	void CameraComponent::OnFovChange()
	{
		m_AttrsChange.set(CameraAttrBits_Pers);
	}

	void CameraComponent::OnSizeChange()
	{
		m_AttrsChange.set(CameraAttrBits_Ortho);
	}
}