#include "ShadowMap.h"
#include "RenderContext.h"

#include "GlobalSettings.h"

#include "gfx/rhi/Texture.h"
#include "gfx/rhi/UniformBlock.h"
#include "gfx/rhi/UniformBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShadowMap"

namespace pio
{
	ShadowMap::ShadowMap(const Ref<RenderContext>& context, const std::string& name)
		: FrameBufferProxy(context, name)
	{
		CreateFrameBuff();
		CreateUBuffer();

		SetMapSize(glm::vec2(m_FrameBuff->Width(), m_FrameBuff->Height()));
	}

	void ShadowMap::OnTick()
	{
		UpdateVirtualCamPose();
		UpdateFrustum();

		if(m_UBlock->AnyChange())
		{
			m_Context.lock()->UploadData(m_UBlock->GetBuffer()->As<void*>(), m_UBlock->GetByteUsed(), m_UBuffer);
			m_UBlock->Invalidate(false);
		}
	}

	void ShadowMap::CreateFrameBuff()
	{
		auto reso = GlobalSettings::ShadowResolution();
		FrameBufferSpecific fboSpec;
		fboSpec.Name = m_Spec.Name;
		fboSpec.Width = reso.x;
		fboSpec.Height = reso.y;
		PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Depth);

		TextureSpecificBuilder depth;
		depth.Name(std::string(m_Spec.Name).append("_DepthBuffer"))
			.Type(TextureType::TwoDimen)
			.Format(TextureFormat::DEPTH_32F)
			.Width(reso.x).Height(reso.y)
			.TexWrap(TextureWrap::ClampBorder, TextureWrap::ClampBorder)
			.Border(glm::vec4(1.f))
			.TexFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);
		fboSpec.DepthSpec.push_back(depth.Build());

		m_FrameBuff = FrameBuffer::Create(m_Context.lock(), fboSpec);
	}

	void ShadowMap::CreateUBuffer()
	{
		m_UBlock = CreateRef<UniformBlock>();
		m_UBlock->PushBack("ViewMat", UniformBlock::CreateData(UniformType::Mat4, "ViewMat"));
		m_UBlock->PushBack("PrjMat", UniformBlock::CreateData(UniformType::Mat4, "PrjMat"));
		m_UBlock->PushBack("MapSize", UniformBlock::CreateData(UniformType::Vec2, "MapSize"));
		m_UBlock->Calculate();

		m_UBuffer = UniformBuffer::Create(m_Context.lock(), m_UBlock->GetByteUsed(),
										  UBBinding_DirectionalLitShadow, BufferUsage::Dynamic);
	}

	void ShadowMap::UpdateVirtualCamPose()
	{
		if(m_bVirtualCamPoseChange)
		{
			m_bVirtualCamPoseChange = false;

			Math::CalcCameraPose(m_ViewDirection, m_VirtualCamRight, m_VirtualCamUp, m_VirtualCamFront);
			m_ViewMat = Math::CameraViewMat(m_VirtualPosition, m_ViewDirection, m_VirtualCamUp);

			auto layout = (*m_UBlock)["ViewMat"];
			m_UBlock->WriteAt(glm::value_ptr(m_ViewMat), sizeof(glm::mat4), layout->GetAlignOffset());
		}
	}

	void ShadowMap::UpdateFrustum()
	{
		if(m_bSceneBoundingChange)
		{
			m_bSceneBoundingChange = false;

			auto bounding = m_ViewMat * m_SceneBoundingBox;
			m_PrjMat = Math::OrthogonalMat(bounding.GetMin().x, bounding.GetMax().x, bounding.GetMin().y, bounding.GetMax().y, 0.1f, 100.f);
			auto layout = (*m_UBlock)["PrjMat"];
			m_UBlock->WriteAt(glm::value_ptr(m_PrjMat), sizeof(glm::mat4), layout->GetAlignOffset());
		}
	}

	void ShadowMap::OnVirtualPositionChange()
	{
		m_bVirtualCamPoseChange = true;
	}

	void ShadowMap::OnViewDirectionChange()
	{
		m_bVirtualCamPoseChange = true;
	}

	void ShadowMap::OnMapSizeChange()
	{
		auto layout = (*m_UBlock)["MapSize"];
		m_UBlock->WriteAt(&m_MapSize, sizeof(glm::vec2), layout->GetAlignOffset());
	}

	void ShadowMap::OnSceneBoundingBoxChange()
	{
		m_bSceneBoundingChange = true;
	}
}