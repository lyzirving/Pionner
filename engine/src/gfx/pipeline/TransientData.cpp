#include "TransientData.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "TransientData"

namespace pio
{
	MeshRenderingItem::MeshRenderingItem(const MeshRenderingItem& rhs)
	{
		Mode = rhs.Mode;
		DrawFlag = rhs.DrawFlag;
		MeshBuff = rhs.MeshBuff;
		Material = rhs.Material;
		UniformBuffers = rhs.UniformBuffers;
	}

	MeshRenderingItem::MeshRenderingItem(MeshRenderingItem&& rhs) noexcept
	{
		Mode = rhs.Mode;
		DrawFlag = std::move(rhs.DrawFlag);
		MeshBuff = std::move(rhs.MeshBuff);
		Material = std::move(rhs.Material);
		UniformBuffers = std::move(rhs.UniformBuffers);

		rhs.Mode = RenderingMode_Num;
	}

	MeshRenderingItem& MeshRenderingItem::operator=(const MeshRenderingItem& rhs)
	{
		if(this != &rhs)
		{
			this->MeshRenderingItem::MeshRenderingItem(rhs);
		}
		return *this;
	}

	MeshRenderingItem& MeshRenderingItem::operator=(MeshRenderingItem&& rhs) noexcept
	{
		if(this != &rhs)
		{
			this->MeshRenderingItem::MeshRenderingItem(std::move(rhs));
		}
		return *this;
	}

	PipelineData::PipelineData(const PipelineData& rhs)
	{
		DirLitShadowMap = rhs.DirLitShadowMap;
		PointLitShadowMap = rhs.PointLitShadowMap;
		GBuffer = rhs.GBuffer;
		DefferedBuffer = rhs.DefferedBuffer;
	}

	PipelineData::PipelineData(PipelineData&& rhs) noexcept
	{
		DirLitShadowMap = std::move(rhs.DirLitShadowMap);
		PointLitShadowMap = std::move(rhs.PointLitShadowMap);
		GBuffer = std::move(rhs.GBuffer);
		DefferedBuffer = std::move(rhs.DefferedBuffer);
	}

	PipelineData& PipelineData::operator=(const PipelineData& rhs)
	{
		if(this != &rhs)
		{
			this->PipelineData::PipelineData(rhs);
		}
		return *this;
	}

	PipelineData& PipelineData::operator=(PipelineData&& rhs) noexcept
	{
		if(this != &rhs)
		{
			this->PipelineData::PipelineData(std::move(rhs));
		}
		return *this;
	}

	RenderingData::RenderingData(const RenderingData& rhs)
	{
		for(size_t i = 0; i < UBBinding_Num; i++)
			UBuffs[i] = rhs.UBuffs[i];
		OpaqueMeshItems = rhs.OpaqueMeshItems;
		TransparentMeshItems = rhs.TransparentMeshItems;
		ShadowCasterItems = rhs.ShadowCasterItems;
		WireframeItems = rhs.WireframeItems;
		SpriteItems = rhs.SpriteItems;
		Pipeline = rhs.Pipeline;
	}

	RenderingData::RenderingData(RenderingData&& rhs) noexcept
	{
		for(size_t i = 0; i < UBBinding_Num; i++)
		{
			UBuffs[i] = rhs.UBuffs[i];
			rhs.UBuffs[i].reset();
		}
		OpaqueMeshItems = std::move(rhs.OpaqueMeshItems);
		TransparentMeshItems = std::move(rhs.TransparentMeshItems);
		ShadowCasterItems = std::move(rhs.ShadowCasterItems);
		WireframeItems = std::move(rhs.WireframeItems);
		SpriteItems = std::move(rhs.SpriteItems);
		Pipeline = std::move(rhs.Pipeline);
	}

	RenderingData& RenderingData::operator=(const RenderingData& rhs)
	{
		if(this != &rhs)
		{
			this->RenderingData::RenderingData(rhs);
		}
		return *this;
	}

	RenderingData& RenderingData::operator=(RenderingData&& rhs) noexcept
	{
		if(this != &rhs)
		{
			this->RenderingData::RenderingData(std::move(rhs));
		}
		return *this;
	}
}