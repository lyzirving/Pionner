#ifndef __PIONNER_GLOBAL_DEF_H__
#define __PIONNER_GLOBAL_DEF_H__

#include <string>

namespace pio
{
	enum PioEntityType : uint8_t
	{
		PIO_ENTITY_GEO,
		PIO_ENTITY_CNT
	};

	struct EntityParam
	{
		PioEntityType type{ PIO_ENTITY_CNT };
		std::string   nodeName{};
	};
}

#endif