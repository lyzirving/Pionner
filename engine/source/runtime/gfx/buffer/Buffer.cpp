#include "Buffer.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Buffer"

namespace pio
{
	namespace gfx
	{
		Buffer::Buffer() : m_id(0), m_bufferType(BUF_CNT), m_uploaded(false)
		{
		}

		Buffer::~Buffer()
		{
		}

		void Buffer::bind()
		{
			//empty implementation by default
		}

		void Buffer::bindTarget(uint32_t target)
		{
			//empty implementation by default
		}

		void Buffer::unbind()
		{
			//empty implementation by default
		}

		void Buffer::load()
		{
			//empty implementation by default
		}

		uint32_t Buffer::size()
		{
			return 0;
		}

		void Buffer::release()
		{
			//empty implementation by default
		}
	}
}