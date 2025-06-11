#include "PointLitGroupComponent.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/renderer/PointLitShadowMap.h"

#include "gfx/rhi/UniformBlock.h"
#include "gfx/rhi/UniformBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PointLitGroupComponent"

namespace pio
{
	void PointLitGroupComponent::OnAttach()
	{
		CreateBuffer();
		CreateShadowMap();
		OnNumChange();
	}

	void PointLitGroupComponent::OnTick()
	{
		if(m_UBlock->AnyChange())
		{
			m_Context.lock()->UploadData(m_UBlock->GetBuffer()->As<void*>(), m_UBlock->GetByteUsed(), m_UBuffer);
			m_UBlock->Invalidate(false);
		}
		m_ShadowMap->OnTick();
	}

	void PointLitGroupComponent::CreateBuffer()
	{
		m_UBlock = CreateRef<UniformBlock>();
		m_UBlock->PushBack("Lits", UniformBlock::CreateData(UniformType::StructArray, "Lits"));
		m_UBlock->PushBack("Num", UniformBlock::CreateData(UniformType::Int, "Num"));

		auto* lits = (*m_UBlock)["Lits"]->As<UniformStructArrayLayout>();
		for(size_t i = 0; i < PIO_POINT_LIT_LIMIT; i++)
		{
			UniformStructLayout l("Lit" + std::to_string(i));
			l.Put(0, UniformBlock::CreateData(UniformType::Vec3, "Position"));
			l.Put(1, UniformBlock::CreateData(UniformType::Vec3, "Color"));
			l.Put(2, UniformBlock::CreateData(UniformType::Int, "Index"));
			l.Put(3, UniformBlock::CreateData(UniformType::Float, "Intensity"));
			l.Put(4, UniformBlock::CreateData(UniformType::Float, "Radius"));
			l.Put(5, UniformBlock::CreateData(UniformType::Float, "Falloff"));
			l.Put(6, UniformBlock::CreateData(UniformType::Float, "Bias"));
			l.Put(7, UniformBlock::CreateData(UniformType::Float, "ShadowIntensity"));
			l.Put(8, UniformBlock::CreateData(UniformType::Int, "ShadowMode"));
			l.Put(9, UniformBlock::CreateData(UniformType::Bool, "CastShadow"));
			lits->PushBack(l);
		}
		m_UBlock->Calculate();
		m_UBuffer = UniformBuffer::Create(m_Context.lock(), m_UBlock->GetByteUsed(),
										  UBBinding_PointLit, BufferUsage::Dynamic);
	}

	void PointLitGroupComponent::CreateShadowMap()
	{
		m_ShadowMap = CreateRef<PointLitShadowMap>(m_Context.lock(), "PointLitShadowMap");
	}

	void PointLitGroupComponent::OnNumChange()
	{
		auto layout = (*m_UBlock)["Num"];
		m_UBlock->WriteAt(&m_Num, sizeof(int32_t), layout->GetAlignOffset());

		m_ShadowMap->SetNum(m_Num);
	}

	void PointLitGroupComponent::OnFovChange()
	{
		m_ShadowMap->SetFov(m_Fov);
	}

	void PointLitGroupComponent::OnNearChange()
	{
		m_ShadowMap->SetNear(m_Near);
	}

	void PointLitGroupComponent::OnFarChange()
	{
		m_ShadowMap->SetFar(m_Far);
	}
}