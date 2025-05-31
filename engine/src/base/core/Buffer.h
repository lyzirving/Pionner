#pragma once

#include "base/TypeDef.h"

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

		void Write(const void *data, uint64_t size);
		void WriteAt(const void *data, uint64_t size, uint64_t offset);
		void Replace(const void *data, uint64_t size, uint64_t offset);		

	private:
		void Extend(uint64_t cap);

	public:
		uint64_t getCapacity() const { return m_capacity; }
		uint64_t getSize() const { return m_offset; }
		bool Valid() const { return m_offset <= m_capacity; }
		bool invalid() const { return !Valid(); }
		void* Data() const { return m_Data; }

		operator bool() const { return m_Data != nullptr; }
		byte &operator[](int index) { return ((byte *)m_Data)[index]; }
		byte operator[](int index) const { return ((byte *)m_Data)[index]; }

	public:
		template<typename T>
		T &read(uint64_t offset = 0)
		{
			return *(T *)((byte *)m_Data + offset);
		}

		template<typename T>
		const T &read(uint64_t offset = 0) const
		{
			return *(T *)((byte *)m_Data + offset);
		}

		template<typename T>
		T *As() const
		{
			return (T *)m_Data;
		}

		template<typename T>
		void move(T** data)
		{			
			*data = As<T>();
			m_Data = nullptr;
			m_capacity = m_offset = 0;
		}

	private:
		void *m_Data{ nullptr };
		uint64_t m_capacity{ 0 };
		uint64_t m_offset{ 0 };
	};
}