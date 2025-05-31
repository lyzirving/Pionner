#pragma once

#include "base/TypeDef.h"

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
		uint8_t *m_Buffer{ nullptr };
		uint8_t *m_BufferPtr{ nullptr };
		uint32_t m_cmdCount{ 0 };
		uint32_t m_occupyBytes{ 0 };
	};

}
