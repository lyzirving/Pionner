#include "UUID.h"

#include <random>

namespace pio
{
	static std::random_device s_randomDevice;
	static std::mt19937_64 eng(s_randomDevice());
	static std::uniform_int_distribution<uint64_t> s_uniformDistribution;

	static std::mt19937 eng32(s_randomDevice());
	static std::uniform_int_distribution<uint32_t> s_uniformDistribution32;

	UUID::UUID() : m_UUID(s_uniformDistribution(eng))
	{
	}

	UUID::UUID(uint64_t uuid) : m_UUID(uuid)
	{
	}

	UUID::UUID(const UUID& other) : m_UUID(other.m_UUID)
	{
	}

	UUID::UUID(UUID&& other) noexcept
	{
		m_UUID = other.m_UUID;
		other.m_UUID = InvalidId;
	}

	UUID& UUID::operator=(const UUID& other)
	{
		if(this != &other)
		{
			this->UUID::UUID(other);
		}
		return *this;
	}

	UUID& UUID::operator=(UUID&& other) noexcept
	{
		if(this != &other)
		{
			this->UUID::UUID(std::forward<UUID>(other));
		}
		return *this;
	}

	bool UUID::operator==(UUID& rhs)
	{
		return m_UUID == rhs.m_UUID;
	}

	bool UUID::operator!=(UUID& rhs)
	{
		return !(*this == rhs);
	}

	bool UUID::operator<(UUID& rhs)
	{
		return m_UUID < rhs.m_UUID;
	}

	bool UUID::operator<=(UUID& rhs)
	{
		return m_UUID <= rhs.m_UUID;
	}


	UUID32::UUID32() : m_UUID(s_uniformDistribution32(eng32))
	{
	}

	UUID32::UUID32(uint32_t uuid) : m_UUID(uuid)
	{
	}

	UUID32::UUID32(const UUID32& other) : m_UUID(other.m_UUID)
	{
	}

	UUID32::UUID32(UUID32&& other) noexcept
	{
		m_UUID = other.m_UUID;
		other.m_UUID = InvalidId;
	}

	UUID32& UUID32::operator=(const UUID32& other)
	{
		if(this != &other)
		{
			this->UUID32::UUID32(other);
		}
		return *this;
	}

	UUID32& UUID32::operator=(UUID32&& other) noexcept
	{
		if(this != &other)
		{
			this->UUID32::UUID32(std::forward<UUID32>(other));
		}
		return *this;
	}

	bool UUID32::operator==(UUID32& rhs)
	{
		return m_UUID == rhs.m_UUID;
	}

	bool UUID32::operator!=(UUID32& rhs)
	{
		return !(*this == rhs);
	}

	bool UUID32::operator<(UUID32& rhs)
	{
		return m_UUID < rhs.m_UUID;
	}

	bool UUID32::operator<=(UUID32& rhs)
	{
		return m_UUID <= rhs.m_UUID;
	}

}