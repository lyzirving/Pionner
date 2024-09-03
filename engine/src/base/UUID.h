#ifndef __PIONNER_BASE_UUID_H__
#define __PIONNER_BASE_UUID_H__

#include <sstream>

namespace pio
{
	constexpr static const uint32_t InvalidId = 0U;

	// "UUID" (universally unique identifier) or GUID is (usually) a 128-bit integer
	// used to "uniquely" identify information. In Pionner, even though we use the term
	// GUID and UUID, at the moment we're simply using a randomly generated 64-bit
	// integer, as the possibility of a clash is low enough for now.
	// This may change in the future.
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID &other);
		UUID(UUID&& other) noexcept;

		UUID& operator=(const UUID& other);
		UUID& operator=(UUID&& other) noexcept;

		bool operator==(UUID &rhs);
		bool operator!=(UUID &rhs);
		bool operator<(UUID &rhs);
		bool operator<=(UUID &rhs);

		operator uint64_t () { return m_UUID; }
		operator const uint64_t() const { return m_UUID; }

		std::string toString() const
		{
			std::stringstream ss;
			ss << "UUID: " << m_UUID;
			return ss.str();
		}

	private:
		uint64_t m_UUID;
	};

	class UUID32
	{
	public:
		UUID32();
		UUID32(uint32_t uuid);
		UUID32(const UUID32 &other);
		UUID32(UUID32&& other) noexcept;

		UUID32& operator=(const UUID32& other);
		UUID32& operator=(UUID32&& other) noexcept;

		bool operator==(UUID32 &rhs);
		bool operator!=(UUID32 &rhs);
		bool operator<(UUID32 &rhs);
		bool operator<=(UUID32 &rhs);

		operator uint32_t () { return m_UUID; }
		operator const uint32_t() const { return m_UUID; }

		std::string toString() const
		{
			std::stringstream ss;
			ss << "UUID32: " << m_UUID;
			return ss.str();
		}

	private:
		uint32_t m_UUID;
	};

}

namespace std
{
	// The following code is used for unordered_map as key
	template <>
	struct hash<pio::UUID>
	{
		std::size_t operator()(const pio::UUID &uuid) const
		{
			// uuid is already a randomly generated number, and is suitable as a hash key as-is.
			// this may change in future, in which case return hash<uint64_t>{}(uuid); might be more appropriate
			return uuid;
		}
	};

	template <>
	struct hash<pio::UUID32>
	{
		std::size_t operator()(const pio::UUID32 &uuid) const
		{
			return hash<uint32_t>()((uint32_t)uuid);
		}
	};
}

#endif
