#include "PointLitShadowMap.h"
#include "RenderContext.h"

#include "GlobalSettings.h"

#include "gfx/rhi/Texture.h"
#include "gfx/rhi/UniformBlock.h"
#include "gfx/rhi/UniformBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PointLitShadowMap"

namespace pio
{
	PointLitShadowMap::PointLitShadowMap(const Ref<RenderContext>& context, const std::string& name)
		: FrameBufferProxy(context, name)
	{
		CreateFrameBuff();
		CreateUBuffer();
		InitAttrs();		
	}	

	void PointLitShadowMap::OnTick()
	{
		if(m_UBlock->AnyChange())
		{
			SetLitSpaceMatData();
			m_Context.lock()->UploadData(m_UBlock->GetBuffer()->As<void*>(), m_UBlock->GetByteUsed(), m_UBuffer);
			m_UBlock->Invalidate(false);
		}
	}

	void PointLitShadowMap::CreateFrameBuff()
	{
		auto resolution = GlobalSettings::ShadowResolution(1.f);

		FrameBufferSpecific fboSpec;
		fboSpec.Name = "PointLitShadowMap";
		fboSpec.Width = resolution.x;
		fboSpec.Height = resolution.y;
		PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Depth);

		TextureSpecificBuilder depth;
		depth.Name("PointLitShadowMapDepthBuffer")
			.Type(TextureType::CubeMapArray)
			.Format(TextureFormat::DEPTH_32F)
			.Width(resolution.x).Height(resolution.y)
			.TexWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge, TextureWrap::ClampEdge)
			.Border(glm::vec4(1.f))
			.TexFilter(TextureFilterMin::Linear, TextureFilterMag::Linear)
			.Num(PIO_POINT_LIT_LIMIT);
		fboSpec.DepthSpec.push_back(depth.Build());

		m_FrameBuff = FrameBuffer::Create(m_Context.lock(), fboSpec);
	}

	void PointLitShadowMap::CreateUBuffer()
	{
		m_UBlock = CreateRef<UniformBlock>();
		m_UBlock->PushBack("LitAttrs", UniformBlock::CreateData(UniformType::StructArray, "LitAttrs"));
		m_UBlock->PushBack("FrustumFar", UniformBlock::CreateData(UniformType::Float, "FrustumFar"));
		m_UBlock->PushBack("LitNum", UniformBlock::CreateData(UniformType::Int, "LitNum"));

		auto* litAtts = (*m_UBlock)["LitAttrs"]->As<UniformStructArrayLayout>();
		for(uint32_t i = 0; i < PIO_POINT_LIT_LIMIT; i++)
		{
			UniformStructLayout s("PointLitShadowCasterAttr" + std::to_string(i));
			s.Put(0, UniformBlock::CreateData(UniformType::Mat4Array, "LitSpaceMat", CubeMapDir_Num));
			s.Put(1, UniformBlock::CreateData(UniformType::Vec3, "Position"));
			s.Put(2, UniformBlock::CreateData(UniformType::Bool, "CastShadow"));
			litAtts->PushBack(s);
		}
		m_UBlock->Calculate();
		m_UBuffer = UniformBuffer::Create(m_Context.lock(), m_UBlock->GetByteUsed(),
										  UBBinding_PointLitShadow, BufferUsage::Dynamic);
	}

	void PointLitShadowMap::InitAttrs()
	{
		for(size_t i = 0; i < CubeMapDir_Num; i++)
		{
			m_LitSpaceMat[i] = glm::mat4(1.f);			
		}

		for(size_t i = 0; i < PIO_POINT_LIT_LIMIT; i++)
		{			
			m_LitPosition[i] = glm::vec3(0.f);
		}

		OnFovChange();
		OnNearChange();
		OnFarChange();
	}	

	void PointLitShadowMap::SetLitPosition(int32_t index, const glm::vec3& position)
	{
		if(index >= m_Num)
		{
			LOGE("err! index[%d] is greater than lit num[%d]", index, m_Num);
			return;
		}
		if(m_LitPosition[index] != position)
		{
			m_LitPosition[index] = position;
			auto* litAtts = (*m_UBlock)["LitAttrs"]->As<UniformStructArrayLayout>();
			auto layout = (*litAtts)[index].Get(1);
			m_UBlock->WriteAt(glm::value_ptr(m_LitPosition[index]), sizeof(glm::vec3), layout->GetAlignOffset());
		}				
	}

	void PointLitShadowMap::SetLitCastShadow(int32_t index, bool cast)
	{
		if(index >= m_Num)
		{
			LOGE("err! index[%d] is greater than lit num[%d]", index, m_Num);
			return;
		}
		auto* litAtts = (*m_UBlock)["LitAttrs"]->As<UniformStructArrayLayout>();
		auto layout = (*litAtts)[index].Get(2);
		m_UBlock->WriteAt(&cast, sizeof(bool), layout->GetAlignOffset());
	}

	void PointLitShadowMap::OnFovChange()
	{		
		CalcLitSpacePrjMat();
		m_UBlock->Invalidate();		
	}

	void PointLitShadowMap::OnNearChange()
	{		
		CalcLitSpacePrjMat();
		m_UBlock->Invalidate();
	}

	void PointLitShadowMap::OnFarChange()
	{		
		CalcLitSpacePrjMat();		
		auto layout = (*m_UBlock)["FrustumFar"];
		m_UBlock->WriteAt(&m_Far, sizeof(float), layout->GetAlignOffset());			
	}

	void PointLitShadowMap::OnNumChange()
	{
		auto layout = (*m_UBlock)["LitNum"];
		m_UBlock->WriteAt(&m_Num, sizeof(int32_t), layout->GetAlignOffset());
	}

	void PointLitShadowMap::CalcLitSpacePrjMat()
	{
		float aspect = float(m_FrameBuff->Width()) / float(m_FrameBuff->Height());
		m_PrjMat = Math::PerspectiveMat(m_Fov, aspect, m_Near, m_Far);
	}

	void PointLitShadowMap::CalcLitSpaceMat(const glm::vec3& position)
	{
		m_LitSpaceMat[CubeMapDir_PositiveX] = m_PrjMat * Math::CameraViewMat(position, World::Right, World::Down);
		m_LitSpaceMat[CubeMapDir_NegativeX] = m_PrjMat * Math::CameraViewMat(position, World::Left, World::Down);
		m_LitSpaceMat[CubeMapDir_PositiveY] = m_PrjMat * Math::CameraViewMat(position, World::Up, World::Forward);
		m_LitSpaceMat[CubeMapDir_NegativeY] = m_PrjMat * Math::CameraViewMat(position, World::Down, World::Back);
		m_LitSpaceMat[CubeMapDir_NegativeZ] = m_PrjMat * Math::CameraViewMat(position, World::Forward, World::Down);
		m_LitSpaceMat[CubeMapDir_PositiveZ] = m_PrjMat * Math::CameraViewMat(position, World::Back, World::Down);
	}

	void PointLitShadowMap::SetLitSpaceMatData()
	{
		auto* litAtts = (*m_UBlock)["LitAttrs"]->As<UniformStructArrayLayout>();
		for(size_t i = 0; i < m_Num; i++)
		{
			auto* litMatLayout = (*litAtts)[i].Get(0)->As<UniformMatArrayLayout>();
			CalcLitSpaceMat(m_LitPosition[i]);
			for(size_t i = 0; i < CubeMapDir_Num; i++)
			{
				auto& layout = (*litMatLayout)[i];
				m_UBlock->WriteAt(glm::value_ptr(m_LitSpaceMat[i]), sizeof(glm::mat4), layout.GetAlignOffset());
			}
		}		
	}
}