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

	UUID::UUID(const UUID &other) : m_UUID(other.m_UUID)
	{
	}

	bool UUID::operator==(UUID &rhs)
	{
		return m_UUID == rhs.m_UUID;
	}

	bool UUID::operator!=(UUID &rhs)
	{
		return !(*this == rhs);
	}

	bool UUID::operator<(UUID &rhs)
	{
		return m_UUID < rhs.m_UUID;
	}

	bool UUID::operator<=(UUID &rhs)
	{
		return m_UUID <= rhs.m_UUID;
	}


	UUID32::UUID32() : m_UUID(s_uniformDistribution32(eng32))
	{
	}

	UUID32::UUID32(uint32_t uuid) : m_UUID(uuid)
	{
	}

	UUID32::UUID32(const UUID32 &other) : m_UUID(other.m_UUID)
	{
	}

	bool UUID32::operator==(UUID32 &rhs)
	{
		return m_UUID == rhs.m_UUID;
	}

	bool UUID32::operator!=(UUID32 &rhs)
	{
		return !(*this == rhs);
	}

	bool UUID32::operator<(UUID32 &rhs)
	{
		return m_UUID < rhs.m_UUID;
	}

	bool UUID32::operator<=(UUID32 &rhs)
	{
		return m_UUID <= rhs.m_UUID;
	}

}