#include "Light.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Light"

namespace pio
{
	UniformBlock DirectionalLightUD::CreateBlock()
	{
		UniformBlock block;
		block.pushBack("Direction", UniformBlock::CreateData(UniformDataType::Vec3, "Direction"));
		block.pushBack("Color", UniformBlock::CreateData(UniformDataType::Vec3, "Radiance"));
		block.pushBack("Intensity", UniformBlock::CreateData(UniformDataType::Float, "Intensity"));
		block.pushBack("Bias", UniformBlock::CreateData(UniformDataType::Float, "Bias"));
		block.pushBack("NormalBias", UniformBlock::CreateData(UniformDataType::Float, "NormalBias"));
		block.pushBack("ShadowMode", UniformBlock::CreateData(UniformDataType::Int, "ShadowMode"));
		block.pushBack("CastShadow", UniformBlock::CreateData(UniformDataType::Bool, "CastShadow"));
		block.pushBack("ShadowIntensity", UniformBlock::CreateData(UniformDataType::Float, "ShadowIntensity"));
		block.calculate();
		//LOGD("block DirectionalLight byte used[%u]", block.getByteUsed());
		return block;
	}

	void DirectionalLightUD::serialize()
	{
		if (!Block.m_buffer || Block.m_buffer->getCapacity() == 0)
		{
			LOGE("DirectionalLight: buffer is invalid");
			return;
		}

		auto directionUD = Block.m_blockItems.get("Direction");
		Block.m_buffer->writeAt(&Direction, sizeof(glm::vec3), directionUD->getAlignOffset());

		auto radianceUD = Block.m_blockItems.get("Color");
		Block.m_buffer->writeAt(&Color, sizeof(glm::vec3), radianceUD->getAlignOffset());

		auto intensityUD = Block.m_blockItems.get("Intensity");
		Block.m_buffer->writeAt(&Intensity, sizeof(float), intensityUD->getAlignOffset());

		auto biasUD = Block.m_blockItems.get("Bias");
		Block.m_buffer->writeAt(&Bias, sizeof(float), biasUD->getAlignOffset());

		auto normalBiasUD = Block.m_blockItems.get("NormalBias");
		Block.m_buffer->writeAt(&NormalBias, sizeof(float), normalBiasUD->getAlignOffset());

		auto sdModeUD = Block.m_blockItems.get("ShadowMode");
		Block.m_buffer->writeAt(&ShadowMode, sizeof(int32_t), sdModeUD->getAlignOffset());

		auto castShadowUD = Block.m_blockItems.get("CastShadow");
		Block.m_buffer->writeAt(&CastShadow, sizeof(bool), castShadowUD->getAlignOffset());

		auto sdIntensityUD = Block.m_blockItems.get("ShadowIntensity");
		Block.m_buffer->writeAt(&ShadowIntensity, sizeof(float), sdIntensityUD->getAlignOffset());
	}
}