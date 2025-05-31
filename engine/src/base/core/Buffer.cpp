#include "Buffer.h"

#include "CoreDef.h"
#include "base/utils/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Buffer"

namespace pio
{
	Buffer::Buffer(uint32_t cap) : m_capacity(cap)
	{
		allocate(0, m_capacity);
	}

	Buffer::Buffer(void *data, uint32_t cap) : m_Data(data), m_capacity(cap), m_offset(cap)
	{
	}

	Buffer::Buffer(const Buffer &rhs)
	{
		if (this != &rhs)
		{
			if(m_Data)
			{
				PIO_FREE(m_Data);
				m_Data = nullptr;
			}
			m_capacity = rhs.m_capacity;
			m_offset = rhs.m_offset;
			if (rhs.m_Data && m_capacity != 0 && (m_Data = PIO_MALLOC(m_capacity)))
			{
				std::memcpy(m_Data, rhs.m_Data, m_capacity);
			}
		}
	}

	Buffer &Buffer::operator=(const Buffer &rhs)
	{
		if (this != &rhs)
		{			
			this->Buffer::Buffer(rhs);
		}
		return *this;
	}

	Buffer::Buffer(Buffer &&rhs) noexcept
	{
		if (this != &rhs)
		{
			m_capacity = rhs.m_capacity;
			m_offset = rhs.m_offset;
			m_Data = rhs.m_Data;

			rhs.m_capacity = rhs.m_offset = 0;
			rhs.m_Data = nullptr;
		}
	}

	Buffer &Buffer::operator=(Buffer &&rhs) noexcept
	{
		if (this != &rhs)
		{
			this->Buffer::Buffer(std::forward<Buffer>(rhs));
		}
		return *this;
	}

	Buffer::~Buffer()
	{
		release();
	}

	void Buffer::allocate(uint64_t cap)
	{
		PIO_FREE(m_Data);
		m_Data = nullptr;

		if (cap == 0)
			return;

		m_Data = PIO_MALLOC(cap);
		m_capacity = cap;
		m_offset = 0;
	}

	void Buffer::allocate(int32_t intializeVal, uint64_t cap)
	{
		allocate(cap);
		if (m_Data && m_capacity != 0)
			std::memset(m_Data, intializeVal, m_capacity);
	}

	Buffer Buffer::copy()
	{
		Buffer buffer;
		buffer.m_Data = PIO_MALLOC(m_capacity);
		buffer.m_capacity = m_capacity;
		buffer.m_offset = m_offset;
		std::memcpy(buffer.m_Data, m_Data, m_capacity);
		return buffer;
	}

	void Buffer::release()
	{
		if (m_Data) { PIO_FREE(m_Data); }		
		m_Data = nullptr;
		m_offset = m_capacity = 0;		 
	}

	void Buffer::reset()
	{
		if (!m_Data)
			return;

		// Invalid state
		if (m_capacity == 0)
		{
			release();
			return;
		}

		m_offset = 0;
	}

	void Buffer::Write(const void *data, uint64_t size)
	{
		if (m_offset + size > m_capacity)
			Extend(m_offset + size);

		std::memcpy((byte *)m_Data + m_offset, data, size);
		m_offset += size;
	}

	void Buffer::WriteAt(const void *data, uint64_t size, uint64_t offset)
	{
		uint64_t occupied = offset + size;
		if (occupied > m_capacity)
			Extend(occupied);

		std::memcpy((byte *)m_Data + offset, data, size);
		if(occupied > m_offset)
			m_offset = occupied;
	}

	void Buffer::Replace(const void *data, uint64_t size, uint64_t offset)
	{
		PIO_CHECK_RETURN(offset + size <= m_capacity,
						  "Buffer overflow, offset[%lu] + size[%lu] > capacity[%lu]", offset, size, m_capacity);
		// Replace() will not change buffer's current offset
		std::memcpy((byte *)m_Data + offset, data, size);
	}

	void Buffer::Extend(uint64_t cap)
	{
		if (cap < m_capacity)
		{
			cap = 2 * m_capacity;
		}

		void *data = PIO_MALLOC(cap);
		if (!data)
		{
			LOGE("fail to malloc [%lu] bytes", cap);
			return;
		}
		std::memcpy(data, m_Data, m_offset);
		std::memset((byte *)data + m_offset, 0, cap - m_offset);

		PIO_FREE(m_Data);
		m_Data = data;
		m_capacity = cap;
	}
}