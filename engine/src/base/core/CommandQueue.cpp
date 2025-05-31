#include "CommandQueue.h"

#include "CoreDef.h"
#include "base/utils/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CommandQueue"

namespace pio
{
	// 10Mb
	#define CMD_BUF_SIZE (10 * 1024 * 1024)

	#define CMD_BUF_OVERFLOW(size) if((size) > CMD_BUF_SIZE) { LOGE("command buffer overflow"); return nullptr; } 

	CommandQueue::CommandQueue()
	{
		m_Buffer = new uint8_t[CMD_BUF_SIZE];
		m_BufferPtr = m_Buffer;
		memset(m_Buffer, 0, CMD_BUF_SIZE);
	}

	CommandQueue::~CommandQueue()
	{
		delete[] m_Buffer;
	}

	void *CommandQueue::allocate(CommandFunc fn, uint32_t execFnSize)
	{
		CMD_BUF_OVERFLOW(m_occupyBytes);

		// turn memory layout to CommandFunc, and copy param fn's value
		*(CommandFunc *)m_BufferPtr = fn;
		// ptr steps forward at bytes occupied by param fn
		m_BufferPtr += sizeof(CommandFunc);
		m_occupyBytes += sizeof(CommandFunc);
		CMD_BUF_OVERFLOW(m_occupyBytes);

		// turn memory layout to uint32_t, and copy param execFnSize's value
		*(uint32_t *)m_BufferPtr = execFnSize;
		// ptr steps forward at bytes occupied by param execFnSize's type size
		m_BufferPtr += sizeof(uint32_t);
		m_occupyBytes += sizeof(uint32_t);
		CMD_BUF_OVERFLOW(m_occupyBytes);

		void *memory = m_BufferPtr;
		// ptr steps forward at execFnSize bytes for next command
		m_BufferPtr += execFnSize;
		m_occupyBytes += execFnSize;
		CMD_BUF_OVERFLOW(m_occupyBytes);

		m_cmdCount++;
		return memory;
	}

	void CommandQueue::execute()
	{
		uint8_t *buffer = m_Buffer;

		for (uint32_t i = 0; i < m_cmdCount; i++)
		{
			CommandFunc function = *(CommandFunc *)buffer;
			buffer += sizeof(CommandFunc);

			uint32_t size = *(uint32_t *)buffer;
			buffer += sizeof(uint32_t);

			// call cmd function
			function(buffer);

			buffer += size;
		}

		m_BufferPtr = m_Buffer;
		m_cmdCount = 0;
		m_occupyBytes = 0;
	}
}