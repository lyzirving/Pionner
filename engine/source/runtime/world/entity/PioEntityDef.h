#ifndef __PIONNER_PIO_ENTITY_DEF_H__
#define __PIONNER_PIO_ENTITY_DEF_H__

#include <cstdint>

namespace pio
{
	enum PioEntityType : uint8_t
	{
		PIO_ENTITY_MODEL,
		PIO_ENTITY_GEO,
		PIO_ENTITY_CNT
	};
}

#endif