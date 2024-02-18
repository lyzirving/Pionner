#ifndef __PIONNER_CORE_UTILS_STRING_UTIL_H__
#define __PIONNER_CORE_UTILS_STRING_UTIL_H__

#include "core/Base.h"

namespace pio
{
	namespace StringUtil
	{
		/*
		* @brief find out whether the src string contains the target ignoring the case
		* @param src: source string
		* @param target: target string
		* @return true means the src contains target, otherwise, not.
		*/
		bool contains(const std::string &src, const std::string &target);
	}
}

#endif