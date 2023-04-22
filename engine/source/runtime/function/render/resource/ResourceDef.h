#ifndef __RESOURCE_DEF_H__
#define __RESOURCE_DEF_H__

#include <cstdint>

namespace Pionner
{
	enum BufferType : uint8_t
	{
		BUF_MEM_ARRAY,
		BUF_VBO,
		BUF_EBO,
		BUF_CNT
	};
}

#endif