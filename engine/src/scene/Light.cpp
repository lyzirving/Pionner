#include "Light.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Light"

namespace pio
{
	UniformBlock DirectionalLight::CreateBlock()
	{
		UniformBlock block;
		block.pushBack("Direction", UniformBlock::CreateData(UniformDataType::Vec3, "Direction"));
		block.pushBack("Radiance", UniformBlock::CreateData(UniformDataType::Vec3, "Radiance"));
		block.pushBack("Intensity", UniformBlock::CreateData(UniformDataType::Float, "Intensity"));
		block.pushBack("Bias", UniformBlock::CreateData(UniformDataType::Float, "Bias"));
		block.pushBack("SdMode", UniformBlock::CreateData(UniformDataType::Int, "SdMode"));
		block.pushBack("CastShadow", UniformBlock::CreateData(UniformDataType::Bool, "CastShadow"));
		block.calculate();
		//LOGD("block DirectionalLight byte used[%u]", block.getByteUsed());
		return block;
	}

	UniformBlock DirectionalLightShadowData::CreateBlock()
	{
		UniformBlock block;
		block.pushBack("ViewMat", UniformBlock::CreateData(UniformDataType::Mat4, "ViewMat"));
		block.pushBack("PrjMat", UniformBlock::CreateData(UniformDataType::Mat4, "PrjMat"));		
		block.calculate();
		//LOGD("block DirectionalLightShadowData byte used[%u]", block.getByteUsed());
		return block;
	}

	UniformBlock PointLight::CreateBlock()
	{
		UniformBlock block;
		block.pushBack("Position", UniformBlock::CreateData(UniformDataType::Vec3, "Position"));
		block.pushBack("Radiance", UniformBlock::CreateData(UniformDataType::Vec3, "Radiance"));
		block.pushBack("Intensity", UniformBlock::CreateData(UniformDataType::Float, "Intensity"));
		block.pushBack("MinRadius", UniformBlock::CreateData(UniformDataType::Float, "MinRadius"));
		block.pushBack("Radius", UniformBlock::CreateData(UniformDataType::Float, "Radius"));
		block.pushBack("Falloff", UniformBlock::CreateData(UniformDataType::Float, "Falloff"));
		block.pushBack("SourceSize", UniformBlock::CreateData(UniformDataType::Float, "SourceSize"));
		block.calculate();
		//LOGD("block PointLight byte used[%u]", block.getByteUsed());
		return block;
	}

	UniformBlock PointLightData::CreateBlock()
	{
		UniformBlock block;
		block.pushBack("LightCount", UniformBlock::CreateData(UniformDataType::Int, "LightCount"));
		block.pushBack("Lights", UniformBlock::CreateData(UniformDataType::StructArray, "Lights"));
		UniformStructArray *lights = block["Lights"]->as<UniformStructArray>();
		for (uint32_t i = 0; i < POINT_LIGHT_LIMIT; i++)
		{
			UniformStruct l("PointLight" + std::to_string(i));
			// Refer to struct PointLight
			l.put(0, UniformBlock::CreateData(UniformDataType::Vec3, "Position"));
			l.put(1, UniformBlock::CreateData(UniformDataType::Vec3, "Radiance"));
			l.put(2, UniformBlock::CreateData(UniformDataType::Float, "Intensity"));
			l.put(3, UniformBlock::CreateData(UniformDataType::Float, "MinRadius"));
			l.put(4, UniformBlock::CreateData(UniformDataType::Float, "Radius"));
			l.put(5, UniformBlock::CreateData(UniformDataType::Float, "Falloff"));
			l.put(6, UniformBlock::CreateData(UniformDataType::Float, "SourceSize"));
			lights->pushBack(l);
		}
		block.calculate();
		//LOGD("block PointLightData byte used[%u]", block.getByteUsed());
		return block;
	}

	UniformBlock PointLightShadowData::CreateBlock()
	{
		UniformBlock block;
		block.pushBack("LightCount", UniformBlock::CreateData(UniformDataType::Int, "LightCount"));
		block.pushBack("Metadata", UniformBlock::CreateData(UniformDataType::StructArray, "Metadata"));
		UniformStructArray *metadata = block["Metadata"]->as<UniformStructArray>();
		for (uint32_t i = 0; i < POINT_LIGHT_LIMIT; i++)
		{
			UniformStruct s("PointLightMetadata" + std::to_string(i));
			s.put(0, UniformBlock::CreateData(UniformDataType::Mat4Array, "LightSpaceMat", 6));
			s.put(1, UniformBlock::CreateData(UniformDataType::Vec3, "Position"));
			s.put(2, UniformBlock::CreateData(UniformDataType::Float, "FrustumFar"));
			metadata->pushBack(s);
		}
		block.calculate();
		//LOGD("block PointLightShadowData byte used[%u]", block.getByteUsed());
		return block;
	}

	void DirectionalLight::serialize()
	{
		if (!Block.m_buffer || Block.m_buffer->getCapacity() == 0)
		{
			LOGE("DirectionalLight: buffer is invalid");
			return;
		}

		auto directionUD = Block.m_blockItems.get("Direction");
		Block.m_buffer->writeAt(&Direction, sizeof(glm::vec3), directionUD->getAlignOffset());

		auto radianceUD = Block.m_blockItems.get("Radiance");
		Block.m_buffer->writeAt(&Radiance, sizeof(glm::vec3), radianceUD->getAlignOffset());

		auto intensityUD = Block.m_blockItems.get("Intensity");
		Block.m_buffer->writeAt(&Intensity, sizeof(float), intensityUD->getAlignOffset());

		auto biasUD = Block.m_blockItems.get("Bias");
		Block.m_buffer->writeAt(&Bias, sizeof(float), biasUD->getAlignOffset());

		auto sdModeUD = Block.m_blockItems.get("SdMode");
		Block.m_buffer->writeAt(&SdMode, sizeof(int32_t), sdModeUD->getAlignOffset());

		auto castShadowUD = Block.m_blockItems.get("CastShadow");
		Block.m_buffer->writeAt(&CastShadow, sizeof(bool), castShadowUD->getAlignOffset());
	}

	void DirectionalLightShadowData::serialize()
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
	}

	void PointLight::serialize()
	{
		if (!Block.m_buffer || Block.m_buffer->getCapacity() == 0)
		{
			LOGE("PointLight: buffer is invalid");
			return;
		}

		auto positionUD = Block.m_blockItems.get("Position");
		Block.m_buffer->writeAt(&Position, sizeof(glm::vec3), positionUD->getAlignOffset());

		auto radianceUD = Block.m_blockItems.get("Radiance");
		Block.m_buffer->writeAt(&Radiance, sizeof(glm::vec3), radianceUD->getAlignOffset());

		auto intensityUD = Block.m_blockItems.get("Intensity");
		Block.m_buffer->writeAt(&Intensity, sizeof(float), intensityUD->getAlignOffset());

		auto minRadiusUD = Block.m_blockItems.get("MinRadius");
		Block.m_buffer->writeAt(&MinRadius, sizeof(float), minRadiusUD->getAlignOffset());

		auto radiusUD = Block.m_blockItems.get("Radius");
		Block.m_buffer->writeAt(&Radius, sizeof(float), radiusUD->getAlignOffset());

		auto falloffUD = Block.m_blockItems.get("Falloff");
		Block.m_buffer->writeAt(&Falloff, sizeof(float), falloffUD->getAlignOffset());

		auto sourceSizeUD = Block.m_blockItems.get("SourceSize");
		Block.m_buffer->writeAt(&SourceSize, sizeof(float), sourceSizeUD->getAlignOffset());
	}

	void PointLightData::serialize()
	{
		if (!Block.m_buffer || Block.m_buffer->getCapacity() == 0)
		{
			LOGE("PointLightData: buffer is invalid");
			return;
		}
		auto lightCntUD = Block["LightCount"];
		Block.m_buffer->writeAt(&LightCount, sizeof(int32_t), lightCntUD->getAlignOffset());

		UniformStructArray *lights = Block["Lights"]->as<UniformStructArray>();
		for (uint32_t i = 0; i < LightCount; i++)
		{
			UniformStruct &s = (*lights)[i];
			auto positionUD = s.getData(0);
			Block.m_buffer->writeAt(&Lights[i].Position, sizeof(glm::vec3), positionUD->getAlignOffset());

			auto radianceUD = s.getData(1);
			Block.m_buffer->writeAt(&Lights[i].Radiance, sizeof(glm::vec3), radianceUD->getAlignOffset());

			auto intensityUD = s.getData(2);
			Block.m_buffer->writeAt(&Lights[i].Intensity, sizeof(float), intensityUD->getAlignOffset());

			auto minRadiusUD = s.getData(3);
			Block.m_buffer->writeAt(&Lights[i].MinRadius, sizeof(float), minRadiusUD->getAlignOffset());

			auto radiusUD = s.getData(4);
			Block.m_buffer->writeAt(&Lights[i].Radius, sizeof(float), radiusUD->getAlignOffset());

			auto falloffUD = s.getData(5);
			Block.m_buffer->writeAt(&Lights[i].Falloff, sizeof(float), falloffUD->getAlignOffset());

			auto sourceSizeUD = s.getData(6);
			Block.m_buffer->writeAt(&Lights[i].SourceSize, sizeof(float), sourceSizeUD->getAlignOffset());
		}
	}

	void PointLightShadowData::serialize()
	{
		if (!Block.m_buffer || Block.m_buffer->getCapacity() == 0)
		{
			LOGE("PointLightShadowData: buffer is invalid");
			return;
		}
		auto lightCntUD = Block["LightCount"];
		Block.m_buffer->writeAt(&LightCount, sizeof(int32_t), lightCntUD->getAlignOffset());

		UniformStructArray *metadata = Block["Metadata"]->as<UniformStructArray>();
		for (uint32_t i = 0; i < LightCount; i++)
		{
			UniformStruct &s = (*metadata)[i];

			auto matArrayUD = s.getData(0);
			UniformMatArray *matArray = matArrayUD->as<UniformMatArray>();
			for (uint32_t j = 0; j < matArray->getArrayNum(); j++)
			{
				UniformMat &m = (*matArray)[j];
				Block.m_buffer->writeAt(glm::value_ptr(Metadata[i].LightSpaceMat[j]), 
										RhiDef::GetUniformByteSize(m.getType()), 
										m.getAlignOffset());
			}

			auto positionUD = s.getData(1);
			Block.m_buffer->writeAt(&Metadata[i].Position, sizeof(glm::vec3), positionUD->getAlignOffset());

			auto frustumFarUD = s.getData(2);
			Block.m_buffer->writeAt(&Metadata[i].FrustumFar, sizeof(float), frustumFarUD->getAlignOffset());
		}
	}
}