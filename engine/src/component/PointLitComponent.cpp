#include "PointLitComponent.h"
#include "PointLitGroupComponent.h"

#include "scene/Scene.h"
#include "scene/node/PointLitNode.h"
#include "scene/node/PointLitGroup.h"

#include "gfx/rhi/UniformBlock.h"
#include "gfx/rhi/UniformBuffer.h"

#include "gfx/renderer/PointLitShadowMap.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PointLitComponent"

namespace pio
{
	void PointLitComponent::OnPositionChange()
	{		
		auto comp = m_Host.lock()->GetScene()->GetPointLitGroup()->GetComponent<PointLitGroupComponent>();
		auto block = comp->GetUBlock();
		auto* lits = (*block)["Lits"]->As<UniformStructArrayLayout>();
		auto layout = (*lits)[m_Index].Get(0);
		block->WriteAt(glm::value_ptr(m_Position), sizeof(glm::vec3), layout->GetAlignOffset());

		auto shadowMap = comp->GetShadowMap();
		shadowMap->SetLitPosition(m_Index, m_Position);
	}

	void PointLitComponent::OnColorChange()
	{
		auto comp = m_Host.lock()->GetScene()->GetPointLitGroup()->GetComponent<PointLitGroupComponent>();
		auto block = comp->GetUBlock();
		auto* lits = (*block)["Lits"]->As<UniformStructArrayLayout>();
		auto layout = (*lits)[m_Index].Get(1);
		block->WriteAt(glm::value_ptr(m_Color), sizeof(glm::vec3), layout->GetAlignOffset());
	}

	void PointLitComponent::OnIndexChange()
	{
		auto comp = m_Host.lock()->GetScene()->GetPointLitGroup()->GetComponent<PointLitGroupComponent>();
		auto block = comp->GetUBlock();
		auto* lits = (*block)["Lits"]->As<UniformStructArrayLayout>();
		auto layout = (*lits)[m_Index].Get(2);
		block->WriteAt(&m_Index, sizeof(int32_t), layout->GetAlignOffset());
	}

	void PointLitComponent::OnIntensityChange()
	{
		auto comp = m_Host.lock()->GetScene()->GetPointLitGroup()->GetComponent<PointLitGroupComponent>();
		auto block = comp->GetUBlock();
		auto* lits = (*block)["Lits"]->As<UniformStructArrayLayout>();
		auto layout = (*lits)[m_Index].Get(3);
		block->WriteAt(&m_Intensity, sizeof(float), layout->GetAlignOffset());
	}

	void PointLitComponent::OnRadiusChange()
	{
		auto comp = m_Host.lock()->GetScene()->GetPointLitGroup()->GetComponent<PointLitGroupComponent>();
		auto block = comp->GetUBlock();
		auto* lits = (*block)["Lits"]->As<UniformStructArrayLayout>();
		auto layout = (*lits)[m_Index].Get(4);
		block->WriteAt(&m_Radius, sizeof(float), layout->GetAlignOffset());
	}

	void PointLitComponent::OnFalloffChange()
	{
		auto comp = m_Host.lock()->GetScene()->GetPointLitGroup()->GetComponent<PointLitGroupComponent>();
		auto block = comp->GetUBlock();
		auto* lits = (*block)["Lits"]->As<UniformStructArrayLayout>();
		auto layout = (*lits)[m_Index].Get(5);
		block->WriteAt(&m_Falloff, sizeof(float), layout->GetAlignOffset());
	}

	void PointLitComponent::OnBiasChange()
	{
		auto comp = m_Host.lock()->GetScene()->GetPointLitGroup()->GetComponent<PointLitGroupComponent>();
		auto block = comp->GetUBlock();
		auto* lits = (*block)["Lits"]->As<UniformStructArrayLayout>();
		auto layout = (*lits)[m_Index].Get(6);
		block->WriteAt(&m_Bias, sizeof(float), layout->GetAlignOffset());
	}

	void PointLitComponent::OnShadowIntensityChange()
	{
		auto comp = m_Host.lock()->GetScene()->GetPointLitGroup()->GetComponent<PointLitGroupComponent>();
		auto block = comp->GetUBlock();
		auto* lits = (*block)["Lits"]->As<UniformStructArrayLayout>();
		auto layout = (*lits)[m_Index].Get(7);
		block->WriteAt(&m_ShadowIntensity, sizeof(float), layout->GetAlignOffset());
	}

	void PointLitComponent::OnModeChange()
	{
		auto comp = m_Host.lock()->GetScene()->GetPointLitGroup()->GetComponent<PointLitGroupComponent>();
		auto block = comp->GetUBlock();
		auto* lits = (*block)["Lits"]->As<UniformStructArrayLayout>();
		auto layout = (*lits)[m_Index].Get(8);
		int32_t sdMode = m_Mode;
		block->WriteAt(&sdMode, sizeof(int32_t), layout->GetAlignOffset());

		bool bCastShadow = (m_Mode != ShadowMode_None) && (m_Mode < ShadowMode_Num);
		SetbCastShadow(bCastShadow);
	}

	void PointLitComponent::OnbCastShadowChange()
	{
		auto comp = m_Host.lock()->GetScene()->GetPointLitGroup()->GetComponent<PointLitGroupComponent>();
		auto block = comp->GetUBlock();
		auto* lits = (*block)["Lits"]->As<UniformStructArrayLayout>();
		auto layout = (*lits)[m_Index].Get(9);
		block->WriteAt(&m_bCastShadow, sizeof(bool), layout->GetAlignOffset());

		auto shadowMap = comp->GetShadowMap();
		shadowMap->SetLitCastShadow(m_Index, m_bCastShadow);
	}

	void PointLitComponent::ApplyAttrsChange()
	{
		OnPositionChange();
		OnColorChange();
		OnIndexChange();
		OnIntensityChange();
		OnRadiusChange();
		OnFalloffChange();
		OnBiasChange();
		OnShadowIntensityChange();
		OnModeChange();
		OnbCastShadowChange();
	}
}
