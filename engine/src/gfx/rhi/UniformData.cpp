#include "UniformData.h"

namespace pio
{
	UniformData::UniformData(UniformDataType type, const std::string& name)
		: m_type(type), m_name(name), m_byteUsed(Rhi::GetUniformByteSize(type))
	{
		m_buffer.allocate(0, m_byteUsed);
	}

	UniformData::UniformData(const UniformData& rhs)
	{
		m_type = rhs.m_type;
		m_name = rhs.m_name;
		m_byteUsed = rhs.m_byteUsed;
		m_buffer = rhs.m_buffer;
	}

	UniformData::UniformData(UniformData&& rhs) noexcept
	{
		m_type = std::move(rhs.m_type);
		m_name = std::move(rhs.m_name);
		m_byteUsed = std::move(rhs.m_byteUsed);
		m_buffer = std::move(rhs.m_buffer);
	}

	UniformData& UniformData::operator=(const UniformData& rhs)
	{
		if (this != &rhs)
		{
			this->UniformData::UniformData(rhs);
		}
		return *this;
	}

	UniformData& UniformData::operator=(UniformData&& rhs) noexcept
	{
		if (this != &rhs)
		{
			this->UniformData::UniformData(std::forward<UniformData>(rhs));
		}
		return *this;
	}

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