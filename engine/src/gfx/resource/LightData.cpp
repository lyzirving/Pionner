#include "LightData.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LightData"

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

	UniformBlock DirectionalLightShadowDataUD::CreateBlock()
	{
		UniformBlock block;
		block.pushBack("ViewMat", UniformBlock::CreateData(UniformDataType::Mat4, "ViewMat"));
		block.pushBack("PrjMat", UniformBlock::CreateData(UniformDataType::Mat4, "PrjMat"));
		block.pushBack("ShadowMapSize", UniformBlock::CreateData(UniformDataType::Vec2, "ShadowMapSize"));
		block.pushBack("FrustumSize", UniformBlock::CreateData(UniformDataType::Float, "FrustumSize"));
		block.calculate();
		//LOGD("block DirectionalLightShadowDataUD byte used[%u]", block.getByteUsed());
		return block;
	}

	void DirectionalLightShadowDataUD::serialize()
	{
		if (!Block.m_buffer || Block.m_buffer->getCapacity() == 0)
		{
			LOGE("DirectionalLightShadowData: UD buffer is invalid");
			return;
		}

		auto viewMatUD = Block.m_blockItems.get("ViewMat");
		Block.m_buffer->writeAt(glm::value_ptr(ViewMat), sizeof(glm::mat4), viewMatUD->getAlignOffset());

		auto prjMatUD = Block.m_blockItems.get("PrjMat");
		Block.m_buffer->writeAt(glm::value_ptr(PrjMat), sizeof(glm::mat4), prjMatUD->getAlignOffset());

		auto sdMapUD = Block.m_blockItems.get("ShadowMapSize");
		Block.m_buffer->writeAt(glm::value_ptr(ShadowMapSize), sizeof(glm::vec2), sdMapUD->getAlignOffset());

		auto frustumSizeUD = Block.m_blockItems.get("FrustumSize");
		Block.m_buffer->writeAt(&FrustumSize, sizeof(float), frustumSizeUD->getAlignOffset());
	}
}