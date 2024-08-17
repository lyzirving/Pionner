#include "CameraData.h"

#include "gfx/core/Camera.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CameraData"

namespace pio
{
	uint32_t CameraData::k_CamDataNum = 0;

	UniformBlock CCameraUD::CreateBlock()
	{
		UniformBlock block;
		block.pushBack("ViewMat", UniformBlock::CreateData(UniformDataType::Mat4, "ViewMat"));
		block.pushBack("PrjMat", UniformBlock::CreateData(UniformDataType::Mat4, "PrjMat"));
		block.pushBack("OrthoMat", UniformBlock::CreateData(UniformDataType::Mat4, "OrthoMat"));
		block.pushBack("CameraPosition", UniformBlock::CreateData(UniformDataType::Vec3, "CameraPosition"));
		block.pushBack("FrustumFar", UniformBlock::CreateData(UniformDataType::Float, "FrustumFar"));
		block.pushBack("PrjType", UniformBlock::CreateData(UniformDataType::Int, "PrjType"));
		block.calculate();
		//LOGD("block CCameraUD byte used[%u]", block.getByteUsed());
		return block;
	}

	void CCameraUD::serialize()
	{
		if(!Block.m_buffer || Block.m_buffer->getCapacity() == 0)
		{
			LOGE("UD buffer is invalid");
			return;
		}

		auto viewMatUD = Block.m_blockItems.get("ViewMat");
		if(viewMatUD)
		{
			Block.m_buffer->writeAt(glm::value_ptr(ViewMat), sizeof(glm::mat4), viewMatUD->getAlignOffset());
		}

		auto prjMatUD = Block.m_blockItems.get("PrjMat");
		if(prjMatUD)
		{
			Block.m_buffer->writeAt(glm::value_ptr(PrjMat), sizeof(glm::mat4), prjMatUD->getAlignOffset());
		}

		auto orthoMatUD = Block.m_blockItems.get("OrthoMat");
		if(orthoMatUD)
		{
			Block.m_buffer->writeAt(glm::value_ptr(OrthoMat), sizeof(glm::mat4), orthoMatUD->getAlignOffset());
		}

		auto cameraPositionUD = Block.m_blockItems.get("CameraPosition");
		if(cameraPositionUD)
		{
			Block.m_buffer->writeAt(&CameraPosition, sizeof(glm::vec3), cameraPositionUD->getAlignOffset());
		}

		auto frustumFarUD = Block.m_blockItems.get("FrustumFar");
		if(frustumFarUD)
		{
			Block.m_buffer->writeAt(&FrustumFar, sizeof(float), frustumFarUD->getAlignOffset());
		}

		auto prjTypeUD = Block.m_blockItems.get("PrjType");
		if(prjTypeUD)
		{
			int32_t val = PrjType;
			Block.m_buffer->writeAt(&val, sizeof(int32_t), prjTypeUD->getAlignOffset());
		}
	}

	CameraData::CameraData() : RenderData(std::string("CameraData") + std::to_string(k_CamDataNum++))
	{
		initData();
	}

	CameraData::CameraData(const std::string &name) : RenderData(name)
	{
		k_CamDataNum++;
		initData();
	}

	CameraData::~CameraData()
	{
	}

	void CameraData::setup(const Camera &camera)
	{		
	}

	void CameraData::initData()
	{
		m_uniformData.obtainBlock();
		m_uniformBuffer = UniformBuffer::Create(m_uniformData.Block.getByteUsed(), (uint32_t)UBBindings::Camera, BufferUsage::DynamicRead);
	}
}