#ifndef __PIONNER_BASE_UTILS_FILE_UTIL_H__
#define __PIONNER_BASE_UTILS_FILE_UTIL_H__

#include "base/defs/FunDef.h"

namespace pio
{
	class FileUtil
	{
		PIO_SINGLETON_DECLARE(FileUtil)
	public:
		static const char* ASSET_ROOT;
		static const char* SEPARATOR;
	};
}

#endif