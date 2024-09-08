#ifndef __PIONNER_BASE_BUFFER_H__
#define __PIONNER_BASE_BUFFER_H__

#include "defs/TypeDef.h"

namespace pio
{
	class Buffer
	{
	public:
		Buffer() {}
		Buffer(uint32_t cap);
		Buffer(void *data, uint32_t cap);

		Buffer(const Buffer &rhs);
		Buffer &operator=(const Buffer &rhs);

		Buffer(Buffer &&rhs) noexcept;
		Buffer &operator=(Buffer &&rhs) noexcept;

		~Buffer();

		void allocate(uint64_t cap);
		void allocate(int32_t intializeVal, uint64_t cap);
		Buffer copy();

		void release();
		void reset();

		void write(const void *data, uint64_t size);
		void writeAt(const void *data, uint64_t size, uint64_t offset);
		void replace(const void *data, uint64_t size, uint64_t offset);		

	private:
		void extend(uint64_t cap);

	public:
		uint64_t getCapacity() const { return m_capacity; }
		uint64_t getSize() const { return m_offset; }
		bool valid() const { return m_offset <= m_capacity; }
		bool invalid() const { return !valid(); }

		operator bool() const { return m_data != nullptr; }
		byte &operator[](int index) { return ((byte *)m_data)[index]; }
		byte operator[](int index) const { return ((byte *)m_data)[index]; }

	public:
		template<typename T>
		T &read(uint64_t offset = 0)
		{
			return *(T *)((byte *)m_data + offset);
		}

		template<typename T>
		const T &read(uint64_t offset = 0) const
		{
			return *(T *)((byte *)m_data + offset);
		}

		template<typename T>
		T *as() const
		{
			return (T *)m_data;
		}

		template<typename T>
		void move(T** data)
		{			
			*data = as<T>();
			m_data = nullptr;
			m_capacity = m_offset = 0;
		}

	private:
		void *m_data{ nullptr };
		uint64_t m_capacity{ 0 };
		uint64_t m_offset{ 0 };
	};
}

#endif