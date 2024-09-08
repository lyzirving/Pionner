#include "UniformData.h"

namespace pio
{
	template<>
	Ref<UniformData> UniformData::Create<bool>(const std::string& name)
	{
		return CreateRef<UniformBool>(name);
	}

	template<>
	Ref<UniformData> UniformData::Create<int32_t>(const std::string& name)
	{
		return CreateRef<UniformInt>(name);
	}

	template<>
	Ref<UniformData> UniformData::Create<uint32_t>(const std::string& name)
	{
		return CreateRef<UniformUInt>(name);
	}

	template<>
	Ref<UniformData> UniformData::Create<float>(const std::string& name)
	{
		return CreateRef<UniformFloat>(name);
	}

	template<>
	Ref<UniformData> UniformData::Create<glm::vec2>(const std::string& name)
	{
		return CreateRef<UniformVec2>(name);
	}

	template<>
	Ref<UniformData> UniformData::Create<glm::vec3>(const std::string& name)
	{
		return CreateRef<UniformVec3>(name);
	}

	template<>
	Ref<UniformData> UniformData::Create<glm::vec4>(const std::string& name)
	{
		return CreateRef<UniformVec4>(name);
	}

	template<>
	Ref<UniformData> UniformData::Create<glm::ivec2>(const std::string& name)
	{
		return CreateRef<UniformIVec2>(name);
	}

	template<>
	Ref<UniformData> UniformData::Create<glm::ivec3>(const std::string& name)
	{
		return CreateRef<UniformIVec3>(name);
	}

	template<>
	Ref<UniformData> UniformData::Create<glm::ivec4>(const std::string& name)
	{
		return CreateRef<UniformIVec4>(name);
	}

	template<>
	Ref<UniformData> UniformData::Create<glm::mat2>(const std::string& name)
	{
		return CreateRef<UniformMat2>(name);
	}

	template<>
	Ref<UniformData> UniformData::Create<glm::mat3>(const std::string& name)
	{
		return CreateRef<UniformMat3>(name);
	}

	template<>
	Ref<UniformData> UniformData::Create<glm::mat4>(const std::string& name)
	{
		return CreateRef<UniformMat4>(name);
	}
}