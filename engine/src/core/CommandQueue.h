#ifndef __PIONNER_CORE_COMMAND_QUEUE_H__
#define __PIONNER_CORE_COMMAND_QUEUE_H__

#include "Base.h"

namespace pio
{

	class CommandQueue
	{
	public:
		typedef void(*CommandFunc)(void *);

		CommandQueue();
		~CommandQueue();

		void *allocate(CommandFunc fn, uint32_t execFnSize);
		void execute();

	private:
		uint8_t *m_buffer{ nullptr };
		uint8_t *m_bufferPtr{ nullptr };
		uint32_t m_cmdCount{ 0 };
		uint32_t m_occupyBytes{ 0 };
	};

}

#endif
