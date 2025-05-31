#include "DirectionalLitComponent.h"
#include "TransformComponent.h"

#include "gfx/rhi/UniformBlock.h"
#include "gfx/rhi/UniformBuffer.h"

#include "gfx/renderer/ShadowMap.h"
#include "gfx/renderer/RenderContext.h"

#include "scene/Scene.h"
#include "scene/node/DirectionalLitNode.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DirectionalLitComponent"

namespace pio
{
	void DirectionalLitComponent::OnAttach()
	{
		CreateShadowMap();
		CreateUBuffer();
		OnDirectionChange();
		OnColorChange();
		OnIntensityChange();
		OnBiasChange();
		OnNormalBiasChange();
		OnModeChange();
		OnShadowIntensityChange();
		OnbCastShadowChange();
	}

	void DirectionalLitComponent::OnTick()
	{
		if(m_UBlock->AnyChange())
		{
			m_Context.lock()->UploadData(m_UBlock->GetBuffer()->As<void*>(), m_UBlock->GetByteUsed(), m_UBuffer);
			m_UBlock->Invalidate(false);
		}

		m_ShadowMap->OnTick();
	}

	void DirectionalLitComponent::OnTransformChange(const Ref<TransformComponent>& comp)
	{
		const auto& rotate = comp->GetRotation();
		SetDirection(glm::normalize(rotate.Mat() * (glm::vec4(-World::Forward, 0.f))));
		
		m_ShadowMap->SetViewDirection(GetDirection());
		auto& boundingBox = m_Host.lock()->GetScene()->GetBoundingBox();
		m_ShadowMap->SetVirtualPosition(boundingBox.Center() - GetDirection() * boundingBox.Diagonal() * 0.5f);
		m_ShadowMap->SetSceneBoundingBox(boundingBox);
	}

	void DirectionalLitComponent::CreateUBuffer()
	{
		m_UBlock = CreateRef<UniformBlock>();
		m_UBlock->PushBack("Direction", UniformBlock::CreateData(UniformType::Vec3, "Direction"));
		m_UBlock->PushBack("Color", UniformBlock::CreateData(UniformType::Vec3, "Color"));
		m_UBlock->PushBack("Intensity", UniformBlock::CreateData(UniformType::Float, "Intensity"));
		m_UBlock->PushBack("Bias", UniformBlock::CreateData(UniformType::Float, "Bias"));
		m_UBlock->PushBack("NormalBias", UniformBlock::CreateData(UniformType::Float, "NormalBias"));
		m_UBlock->PushBack("ShadowMode", UniformBlock::CreateData(UniformType::Int, "ShadowMode"));
		m_UBlock->PushBack("ShadowIntensity", UniformBlock::CreateData(UniformType::Float, "ShadowIntensity"));
		m_UBlock->PushBack("CastShadow", UniformBlock::CreateData(UniformType::Bool, "CastShadow"));
		m_UBlock->Calculate();

		m_UBuffer = UniformBuffer::Create(m_Context.lock(), m_UBlock->GetByteUsed(),
										  UBBinding_DirectionalLit, BufferUsage::Dynamic);		
	}

	void DirectionalLitComponent::CreateShadowMap()
	{
		m_ShadowMap = CreateRef<ShadowMap>(m_Context.lock(), "DirLitShadowMap");
	}

	void DirectionalLitComponent::OnDirectionChange()
	{
		auto layout = (*m_UBlock)["Direction"];
		m_UBlock->WriteAt(glm::value_ptr(m_Direction), sizeof(glm::vec3), layout->GetAlignOffset());
	}

	void DirectionalLitComponent::OnColorChange()
	{
		auto layout = (*m_UBlock)["Color"];
		m_UBlock->WriteAt(glm::value_ptr(m_Color), sizeof(glm::vec3), layout->GetAlignOffset());
	}

	void DirectionalLitComponent::OnIntensityChange()
	{
		auto layout = (*m_UBlock)["Intensity"];
		m_UBlock->WriteAt(&m_Intensity, sizeof(float), layout->GetAlignOffset());
	}

	void DirectionalLitComponent::OnBiasChange()
	{
		auto layout = (*m_UBlock)["Bias"];
		m_UBlock->WriteAt(&m_Bias, sizeof(float), layout->GetAlignOffset());
	}

	void DirectionalLitComponent::OnNormalBiasChange()
	{
		auto layout = (*m_UBlock)["NormalBias"];
		m_UBlock->WriteAt(&m_NormalBias, sizeof(float), layout->GetAlignOffset());
	}

	void DirectionalLitComponent::OnModeChange()
	{
		auto layout = (*m_UBlock)["ShadowMode"];
		int32_t mode = m_Mode;
		m_UBlock->WriteAt(&mode, sizeof(int32_t), layout->GetAlignOffset());

		bool bCastShadow = (m_Mode != ShadowMode_None) && (m_Mode < ShadowMode_Num);
		SetbCastShadow(bCastShadow);
	}

	void DirectionalLitComponent::OnShadowIntensityChange()
	{
		auto layout = (*m_UBlock)["ShadowIntensity"];
		m_UBlock->WriteAt(&m_ShadowIntensity, sizeof(float), layout->GetAlignOffset());
	}

	void DirectionalLitComponent::OnbCastShadowChange()
	{
		auto layout = (*m_UBlock)["CastShadow"];
		m_UBlock->WriteAt(&m_bCastShadow, sizeof(bool), layout->GetAlignOffset());
	}
}